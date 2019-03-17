#include "recorder.h"


Recorder::Recorder(nFrame buffer_size, int cacheBufferCount)
{
    _bufferSize = buffer_size;
    _cacheSize = cacheBufferCount * buffer_size * CHANNEL_COUNT;
    _cache.resize(_cacheSize);

    auto now = std::time(nullptr);
    auto nowLocal = *std::localtime(&now);
    std::ostringstream stringStream;
    stringStream << std::put_time(&nowLocal, RECORDER_OUTPUT_FILE);
    _filepath = stringStream.str();
}

void Recorder::start()
{
    _isRunning = true;
    _thread = std::thread(mainLoop, this);
    _thread.detach();
}

void Recorder::write(Sample * buffer)
{
    nFrame cache_index = 0;

    for( uint i = 0; i < _bufferSize * CHANNEL_COUNT; i++ )
    {
        cache_index = (_writeIndex + i) % _cacheSize;
        _cache[cache_index] = buffer[i];
    }

    _writeIndex += _bufferSize * CHANNEL_COUNT;
}

void Recorder::mainLoop(Recorder* recorder)
{
    nFrame fileIndex = 0;
    nFrame cacheIndex = 0;
    nFrame bufferSize = recorder->bufferSize();
    Sample buf[CHANNEL_COUNT];
    auto sleepDuration = std::chrono::milliseconds(RECORDER_SLEEP_MS);

    std::cout << "Opening output file " << recorder->filepath() << std::endl;

    SndfileHandle outputFile(
        recorder->filepath(),
        SFM_WRITE,
        SF_FORMAT_WAV | SF_FORMAT_PCM_16,
        CHANNEL_COUNT,
        SAMPLE_RATE
    );

    if( outputFile.error() != 0) {
        std::cout << " -> ERROR : " << outputFile.strError() << std::endl;
        return;
    }

    while( recorder->isRunning() )
    {
        if( fileIndex < recorder->writeIndex() )
        {
            for( uint i = 0; i < bufferSize; i++ )
            {
                cacheIndex = fileIndex + (i * CHANNEL_COUNT);

                buf[0] = recorder->cache(cacheIndex);
                buf[1] = recorder->cache(cacheIndex + 1);

                if( outputFile.error() == SF_ERR_NO_ERROR )
                    outputFile.write(buf, CHANNEL_COUNT);
            }

            fileIndex += bufferSize * CHANNEL_COUNT;

        } else {
            std::this_thread::sleep_for(sleepDuration);
        }
    }

    std::cout << "Output file closed" << std::endl;
}
