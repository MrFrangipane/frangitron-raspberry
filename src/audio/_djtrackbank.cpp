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
    std::vector<Sample> peaks;
    peaks.resize(280);

    // PEAKS
    std::string peak_filepath = djTrack.filepath + ".frangipeaks";
    struct stat buf;
    if( stat(peak_filepath.c_str(), &buf) == 0 )
    {
        // READ
        std::fstream f_peaks(peak_filepath);
        std::string line;
        int index = 0;

        while (std::getline(f_peaks, line))
        {
            peaks[index] = std::stod(line);
            index++;
        }
    }
    else
    {
        // WRITE
        int stepFrame = f_audio.frames() / 280;
        nFrame frame = 0;
        Sample buf[PEAK_FRAME_COUNT * f_audio.channels()];
        Sample value = 0;
        std::ofstream f_peaks(peak_filepath);

        if( !f_peaks.is_open() ) {
            std::cout << "Impossible to open " << peak_filepath << std::endl;
            return;
        }

        for( int step = 0; step < 280;  step++ )
        {
            frame = step * stepFrame;
            f_audio.seek(frame, SEEK_SET);
            f_audio.readf(buf, PEAK_FRAME_COUNT);
            for( int i = 0; i < PEAK_FRAME_COUNT; i++ )
            {
                value += buf[i * f_audio.channels()] * buf[i * f_audio.channels()];
            }
            value /= PEAK_FRAME_COUNT;
            value = sqrt(value);

            f_peaks << value << std::endl;
            peaks[step] = value;
        }
        f_peaks.close();
    }

    _peaks.push_back(peaks);
}

void DjTrackBank::registerDjDeck(DjDeckInfos djDeckInfos)
{
    _samples.push_back(std::vector<Sample>());
    _samples.back().resize(DECK_LENGTH_SECONDS * CHANNEL_COUNT * SAMPLE_RATE);
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
    AudioFileInfos track;
    auto sleepDuration = std::chrono::milliseconds(DJTRACK_BANK_SLEEP_DURATION);

    while( trackBank->isRunning() ) {
        for( int deckIndex = 0; deckIndex < trackBank->deckCount(); deckIndex++ ) {
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
