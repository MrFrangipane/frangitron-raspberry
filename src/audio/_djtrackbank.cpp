#include "_djtrackbank.h"

DjTrackBank::DjTrackBank() { }

void DjTrackBank::registerDjTrack(ConfAudioFile configDjTrack)
{
    std::cout << "Registering DjTrack " << configDjTrack.filepath << std::endl;

    SndfileHandle file(configDjTrack.filepath);
    if( file.error() != 0 )
        std::cout << file.strError() << std::endl;

    configDjTrack.frameCount = file.frames();
    configDjTrack.channelCount = file.channels();

    _tracks.push_back(configDjTrack);
}

void DjTrackBank::registerDjDeck(DjDeckInfos djDeckInfos)
{
    _samples.push_back(std::vector<Sample>());
    _samples.back().resize(DECK_LENGTH_SECONDS * CHANNEL_COUNT * SAMPLE_RATE);
    djDeckInfos.startPointer = _samples.back().data();
    _decks.push_back(djDeckInfos);
}

void DjTrackBank::start()
{
    _isRunning = true;
    _thread = std::thread(mainLoop, this);
    _thread.detach();
}

Sample DjTrackBank::sample(int deckIndex, int sampleIndex)
{
    if( _decks.at(deckIndex).needsLoading )
        return 0.0;

    return _samples[deckIndex][sampleIndex];
}

void DjTrackBank::mainLoop(DjTrackBank *trackBank)
{
    DjDeckInfos deck;
    ConfAudioFile track;
    auto sleepDuration = std::chrono::milliseconds(DJTRACK_BANK_SLEEP_DURATION);

    while( trackBank->isRunning() ) {
        for( int deckIndex = 0; deckIndex < trackBank->deckCount(); deckIndex++ ) {
            deck = trackBank->deckInfos(deckIndex);
            if( !deck.needsLoading || deck.startPointer == nullptr )
                continue;

            track = trackBank->trackInfos(deck.trackIndex);
            SndfileHandle file(track.filepath);

            file.read(deck.startPointer, track.channelCount * track.frameCount);

            deck.needsLoading = false;
            trackBank->setDeckInfos(deckIndex, deck);
        }

        std::this_thread::sleep_for(sleepDuration);
    }
}
