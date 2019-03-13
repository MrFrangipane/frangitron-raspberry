#include "recorder.h"


Recorder::Recorder(nFrame buffer_size, int cacheBufferCount)
{
    _bufferSize = buffer_size;
    _cacheSize = cacheBufferCount * buffer_size * 2;
    _cache.reserve(_cacheSize);

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

void Recorder::write(Sample *buffer)
{
    nFrame cache_index;

    for( uint i = 0; i < _bufferSize * 2; i++ )
    {
        cache_index = (_writeIndex + i) % _cacheSize;
        _cache[cache_index] = buffer[i];
    }

    _writeIndex += _bufferSize * 2;
}

void Recorder::mainLoop(Recorder* recorder)
{
    nFrame writeIndex_ = 0;
    nFrame fileIndex = 0;
    nFrame cacheIndex = 0;
    nFrame bufferSize = recorder->bufferSize();
    Sample buf[2];
    int sleepDuration = SECOND_PER_SAMPLE * bufferSize * 500; // half a buffer in milliseconds

    std::cout << "Opening output file " << recorder->filepath() << std::endl;

    SndfileHandle outputFile(
        recorder->filepath(),
        SFM_WRITE,
        SF_FORMAT_WAV | SF_FORMAT_PCM_16,
        2,
        SAMPLE_RATE
    );

    if( outputFile.error() != 0) {
        std::cout << " -> ERROR : " << outputFile.strError() << std::endl;
        return;
    }

    while( recorder->isRunning() )
    {
        writeIndex_ = recorder->writeIndex();
        if( fileIndex < writeIndex_ )
        {
            for( uint i = 0; i < bufferSize; i++ )
            {
                cacheIndex = fileIndex + (i * 2);

                buf[0] = recorder->cache(cacheIndex);
                buf[1] = recorder->cache(cacheIndex + 1);

                if( outputFile.error() == SF_ERR_NO_ERROR )
                    outputFile.write(buf, 2);
            }
            fileIndex += bufferSize * 2;
        } else {
            // sleep for half a buffer
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepDuration));
        }
    }

    std::cout << "Output file closed" << std::endl;
}
