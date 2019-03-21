#include "_djtrackbank.h"


DjTrackBank::DjTrackBank() { }

void DjTrackBank::registerAudioFile(AudioFileInfos djTrack)
{
    // REGISTER
    std::cout << "Registering DjTrack " << djTrack.filepath << std::endl;

    SndfileHandle f_audio(djTrack.filepath);
    if( f_audio.error() != 0 )
        std::cout << f_audio.strError() << std::endl;

    djTrack.frameCount = f_audio.frames();
    djTrack.channelCount = f_audio.channels();

    _tracks.push_back(djTrack);

    // PEAKS
    nFrame frame = 0;
    Sample value = 0;
    std::vector<Sample> peaks;
    peaks.resize(PEAK_IMAGE_WIDTH);
    int stepFrame = f_audio.frames() / PEAK_IMAGE_WIDTH;
    nSample rms_frame_count = (int)((float)stepFrame * 0.25);
    Sample buf[rms_frame_count * f_audio.channels()];

    for( int step = 0; step < PEAK_IMAGE_WIDTH;  step++ )
    {
        frame = step * stepFrame;
        f_audio.seek(frame, SEEK_SET);
        f_audio.readf(buf, rms_frame_count);
        for( int i = 0; i < rms_frame_count; i++ )
        {
            value += buf[i * f_audio.channels()] * buf[i * f_audio.channels()];
        }
        value /= rms_frame_count;
        value = sqrt(value);

        peaks[step] = value;
    }

    _peaks.push_back(peaks);
}

DjDeckInfos DjTrackBank::registerDjDeck(DjDeckInfos djDeckInfos)
{
    _samples.push_back(std::vector<Sample>());
    _samples.back().resize(DECK_LENGTH_SECONDS * CHANNEL_COUNT * SAMPLE_RATE);

    djDeckInfos.index = _decks.size();
    _decks.push_back(djDeckInfos);

    return djDeckInfos;
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
    AudioFileInfos track;
    auto sleepDuration = std::chrono::milliseconds(DJTRACK_BANK_SLEEP_DURATION);

    while( trackBank->isRunning() ) {
        for( int deckIndex = 0; deckIndex < trackBank->deckCount() - 1; deckIndex++ ) {
            deck = trackBank->deckInfos(deckIndex);
            if( !deck.needsLoading )
                continue;

            track = trackBank->trackInfos(deck.trackIndex);
            SndfileHandle file(track.filepath);

            file.read(
                trackBank->pointerToSample(deckIndex),
                track.channelCount * track.frameCount
            );

            deck.needsLoading = false;
            trackBank->setDeckInfos(deckIndex, deck);
        }

        std::this_thread::sleep_for(sleepDuration);
    }
}
