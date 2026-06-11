//***************************************************************************
//
// ALSAWAVEREADER.HPP
//
// Header for ALSAWaveReader, which captures audio via the ALSA API.
// Linux replacement for WinWaveReader.
//
//***************************************************************************

#ifndef __ALSAWAVEREADER_HPP
#define __ALSAWAVEREADER_HPP

#include "../audioreader.hpp"

//***************************************************************************
class ALSAWaveReader : public AudioReader {
private:
    unsigned int AudioLength;

public:
    ALSAWaveReader (unsigned int SampleRate, float Length);

    void NewSampleRate (unsigned int NewRate);
    void NewMediaLength (float NewLength);

    virtual Signal& AudioData (void)
        throw (AudioReaderException);
};

#endif

//************************** ALSAWAVEREADER.HPP *****************************
