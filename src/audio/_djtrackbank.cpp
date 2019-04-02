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
    audioFile.cueCount = 1; // Begining is first cue
    audioFile.cues[0].position = 0;
    audioFile.cues[0].imagePosition = 0;

    SF_CUES * cueInfo = new SF_CUES();
    f_audio.command(SFC_GET_CUE, cueInfo, sizeof(SF_CUES));
    if( cueInfo->cue_count > 0 )
    {
        audioFile.cueCount += cueInfo->cue_count;

        for( uint32_t fileCueIndex = 0; fileCueIndex < cueInfo->cue_count; fileCueIndex++ )
        {
            audioFile.cues[fileCueIndex + 1].position = cueInfo->cue_points[fileCueIndex].position;

            float relativePosition = (float)audioFile.cues[fileCueIndex + 1].position / (float)audioFile.frameCount;
            audioFile.cues[fileCueIndex + 1].imagePosition = relativePosition * PEAK_IMAGE_WIDTH;
        }

        // Hacky ~ (this cue will never be used, but makes region calculations generic)
        audioFile.cues[cueInfo->cue_count + 1].position = audioFile.frameCount - 1;
        audioFile.cues[cueInfo->cue_count + 1].imagePosition = PEAK_IMAGE_WIDTH - 1;
    }
    delete cueInfo;

    // PEAKS
    audioFile.peaks.resize(PEAK_IMAGE_WIDTH);
    std::string peaksFilepath = audioFile.filepath + ".frangipeaks";
    struct stat buf;
#ifdef RASPBERRYPI
    if( stat(peaksFilepath.c_str(), &buf) == 0)
#else // HACK POTTER !! (frangipeak file reading doesnt work on desktop computer)
    if( stat(peaksFilepath.c_str(), &buf) == 0 && false)
#endif
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
            // TAKE A BREATH
            std::this_thread::sleep_for(sleepDuration);

            deck = trackBank->deckInfos(deckIndex);
            if( !deck.needsLoading || deck.audioFileIndex < 0 )
                continue;

            audioFile = trackBank->audioFileInfos(deck.audioFileIndex);
            SndfileHandle f_audio(audioFile.filepath);

            f_audio.read(
                trackBank->_pointerToDeckData(deck.index),
                audioFile.channelCount * audioFile.frameCount
            );

            // TAKE A BREATH
            std::this_thread::sleep_for(sleepDuration);

            deck.needsLoading = false;
            trackBank->setDeckInfos(deckIndex, deck);
        }
    }
}
