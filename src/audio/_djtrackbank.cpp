#include "_djtrackbank.h"


DjTrackBank::DjTrackBank() { }

void DjTrackBank::registerAudioFile(AudioFileInfos audioFile)
{
    // CHANNEL & DURATION
    std::cout << "Registering DjTrack " << audioFile.filepath << std::endl;

    SndfileHandle f_audio(audioFile.filepath);
    if( f_audio.error() != 0 )
        std::cout << f_audio.strError() << std::endl;

    audioFile.frameCount = f_audio.frames();
    audioFile.channelCount = f_audio.channels();

    // CUES
    SF_CUES * cueInfo = new SF_CUES();
    f_audio.command(SFC_GET_CUE, cueInfo, sizeof(SF_CUES));
    if( cueInfo->cue_count > 0 )
    {
        audioFile.cueCount = cueInfo->cue_count;

        for( uint32_t cueIndex = 0; cueIndex < cueInfo->cue_count; cueIndex++ )
        {
            audioFile.cues[cueIndex].position = cueInfo->cue_points[cueIndex].position;

            float relativePosition = (float)audioFile.cues[cueIndex].position / (float)audioFile.frameCount;
            audioFile.cues[cueIndex].imagePosition = relativePosition * PEAK_IMAGE_WIDTH;
        }
    }
    delete cueInfo;

    // PEAKS
    audioFile.peaks.resize(PEAK_IMAGE_WIDTH);
    std::string peaksFilepath = audioFile.filepath + ".frangipeaks";
    struct stat buf;
    if( stat(peaksFilepath.c_str(), &buf) == 0 )
    {
        // READ
        std::fstream f_peaks(peaksFilepath);
        std::string line;
        int index = 0;

        while (std::getline(f_peaks, line))
        {
            audioFile.peaks.at(index) = std::stod(line);
            index++;
        }
    }
    else
    {
        // WRITE
        nFrame frame = 0;
        Sample value = 0;
        int stepFrame = f_audio.frames() / PEAK_IMAGE_WIDTH;
        nSample rmsFrameCount = (int)((float)stepFrame * PEAK_RMS_RATIO);
        Sample buf[rmsFrameCount * f_audio.channels()];
        std::ofstream f_peaks(peaksFilepath);

        if( !f_peaks.is_open() ) {
            std::cout << "Impossible to open " << peaksFilepath << std::endl;
            return;
        }

        for( int step = 0; step < PEAK_IMAGE_WIDTH;  step++ )
        {
            frame = step * stepFrame;
            f_audio.seek(frame, SEEK_SET);
            f_audio.readf(buf, rmsFrameCount);
            for( nSample i = 0; i < rmsFrameCount; i++ )
            {
                value += buf[i * f_audio.channels()] * buf[i * f_audio.channels()];
            }
            value /= rmsFrameCount;
            value = sqrt(value);

            f_peaks << value << std::endl;
            audioFile.peaks[step] = value;
        }
        f_peaks.close();
    }

    // SAVE
    _tracksInfos.push_back(audioFile);
}

DjDeckInfos DjTrackBank::registerDjDeck(DjDeckInfos djDeckInfos)
{
    _decksData.push_back(Buffer());
    _decksData.back().resize(DECK_LENGTH_SECONDS * CHANNEL_COUNT * SAMPLE_RATE);

    djDeckInfos.index = _decks.size();
    _decks.push_back(djDeckInfos);

    return djDeckInfos;
}

void DjTrackBank::start()
{
    _isRunning = true;
    _thread = std::thread(_mainLoop, this);
    _thread.detach();
}

Sample DjTrackBank::sample(int deckIndex, int sampleIndex)
{
    if( _decks.at(deckIndex).needsLoading || _decks.at(deckIndex).audioFileIndex < 0 )
        return 0.0;

    return _decksData.at(deckIndex).at(sampleIndex);
}

void DjTrackBank::_mainLoop(DjTrackBank *trackBank)
{
    DjDeckInfos deck;
    AudioFileInfos audioFile;
    auto sleepDuration = std::chrono::milliseconds(DJTRACK_BANK_SLEEP_DURATION);

    while( trackBank->isRunning() ) {
        for( int deckIndex = 0; deckIndex < trackBank->deckCount(); deckIndex++ ) {
            deck = trackBank->deckInfos(deckIndex);
            if( !deck.needsLoading || deck.audioFileIndex < 0 )
                continue;

            audioFile = trackBank->audioFileInfos(deck.audioFileIndex);
            SndfileHandle f_audio(audioFile.filepath);

            f_audio.read(
                trackBank->_pointerToDeckData(deck.index),
                audioFile.channelCount * audioFile.frameCount
            );

            deck.needsLoading = false;
            trackBank->setDeckInfos(deckIndex, deck);
        }

        std::this_thread::sleep_for(sleepDuration);
    }
}
