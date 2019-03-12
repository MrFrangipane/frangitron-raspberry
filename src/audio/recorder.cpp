#include "recorder.h"


Recorder::Recorder(nFrame buffer_size, int cache_buffer_count)
{
    _buffer_size = buffer_size;
    _cache_size = cache_buffer_count * buffer_size * 2;
    _cache.reserve(_cache_size);

    auto now = std::time(nullptr);
    auto nowLocal = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&nowLocal, RECORDER_OUTPUT_FILE);
    _filepath = oss.str();
}

void Recorder::start()
{
    _running = true;
    _thread = std::thread(main_loop, this);
    _thread.detach();
}

void Recorder::write(Sample *buffer)
{
    nFrame cache_index;

    for( uint i = 0; i < _buffer_size * 2; i++ )
    {
        cache_index = (_write_index + i) % _cache_size;
        _cache[cache_index] = buffer[i];
    }

    _write_index += _buffer_size * 2;
}

void Recorder::main_loop(Recorder* recorder)
{
    nFrame write_index_ = 0;
    nFrame file_index = 0;
    nFrame cache_index = 0;
    nFrame buffer_size = recorder->buffer_size();
    Sample buf[2];
    int sleep_duration = SECOND_PER_SAMPLE * buffer_size * 500; // half a buffer in milliseconds

    std::cout << "Opening output file " << recorder->filepath() << std::endl;

    SndfileHandle output_file(
        recorder->filepath(),
        SFM_WRITE,
        SF_FORMAT_WAV | SF_FORMAT_PCM_16,
        2,
        SAMPLE_RATE
    );

    if( output_file.error() != 0) {
        std::cout << " -> ERROR : " << output_file.strError() << std::endl;
        return;
    }

    while( recorder->running() )
    {
        write_index_ = recorder->write_index();
        if( file_index < write_index_ )
        {
            for( uint i = 0; i < buffer_size; i++ )
            {
                cache_index = file_index + (i * 2);

                buf[0] = recorder->cache(cache_index);
                buf[1] = recorder->cache(cache_index + 1);

                if( output_file.error() == SF_ERR_NO_ERROR )
                    output_file.write(buf, 2);
            }
            file_index += buffer_size * 2;
        } else {
            // sleep for half a buffer
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_duration));
        }
    }

    std::cout << "Output file closed" << std::endl;
}
