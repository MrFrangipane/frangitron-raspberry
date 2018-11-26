#include "filter.h"

double _SampleFilter::process(Sample input)
{
    _buf0 += _realCutoff * (input - _buf0 + _feedbackAmount * (_buf0 - _buf2));
    _buf1 += _realCutoff * (_buf0 - _buf1);
    _buf2 += _realCutoff * (_buf1 - _buf2);
    _buf3 += _realCutoff * (_buf2 - _buf3);

    switch (_mode)
    {
        case LOWPASS:
            return _buf3 / 2.0;

        case HIPASS:
            return (input - _buf3) / 2.0;

        case BANDPASS:
            return (_buf0 - _buf2) / 2.0;

        default:
            return input / 2.0;
    }
}

void Filter::update(FilterStatus status_)
{
    if( status_.cutoff < 0 ) {
        _mode = FilterMode::LOWPASS;
        _lowPassL.setCutoff(1.0 - status_.cutoff);
        _lowPassR.setCutoff(1.0 - status_.cutoff);
        _hiPassL.setCutoff(0.0);
        _hiPassR.setCutoff(0.0);
    }
    else if ( status_.cutoff > 0 ) {
        _mode = FilterMode::HIPASS;
        _lowPassL.setCutoff(1.0);
        _lowPassR.setCutoff(1.0);
        _hiPassL.setCutoff(status_.cutoff);
        _hiPassR.setCutoff(status_.cutoff);
    }
    else {
        _mode = FilterMode::PASSTHROUGH;
    }

    _lowPassL.setResonance(status_.resonance);
    _lowPassR.setResonance(status_.resonance);
    _hiPassL.setResonance(status_.resonance);
    _hiPassR.setResonance(status_.resonance);
}

FilterStatus Filter::status()
{
    FilterStatus status_;

    if( _mode == FilterMode::HIPASS ) status_.cutoff = _hiPassL.cutoff();
    else if( _mode == FilterMode::LOWPASS ) status_.cutoff = _lowPassL.cutoff() - 1.0;
    else status_.cutoff = 0.0;

    status_.resonance = _hiPassL.resonance();

    return status_;
}

void Filter::process(Sample const * bufferIn, const nFrame time)
{
    switch (_mode) {

    case FilterMode::LOWPASS:
        for( nFrame i = 0; i < _bufferSize; i++ ) {
            _left = i * 2;
            _right = _left + 1;

            _bufferOut[_left] = _lowPassL.process(bufferIn[_left]);
            _bufferOut[_right] = _lowPassR.process(bufferIn[_right]);
        }
        break;

    case FilterMode::HIPASS:
        for( nFrame i = 0; i < _bufferSize; i++ ) {
            _left = i * 2;
            _right = _left + 1;

            _bufferOut[_left] = _hiPassL.process(bufferIn[_left]);
            _bufferOut[_right] = _hiPassR.process(bufferIn[_right]);
        }
        break;

    default:
        for( nFrame i = 0; i < _bufferSize; i++ ) {
            _left = i * 2;
            _right = _left + 1;

            _bufferOut[_left] = bufferIn[_left];
            _bufferOut[_right] = bufferIn[_right];
        }
        break;
    }
}

