//***************************************************************************
//
// ALSAWAVEWRITER.HPP
//
// Header for ALSAWaveWriter, which plays audio via the ALSA API.
// Linux replacement for WinWaveWriter.
//
//***************************************************************************

#ifndef __ALSAWAVEWRITER_HPP
#define __ALSAWAVEWRITER_HPP

#include "../../signal/signal.hpp"
#include "../audiowriter.hpp"

//***************************************************************************
class ALSAWaveWriter : public AudioWriter {
private:
    unsigned short int NChannels;

public:
    ALSAWaveWriter (unsigned int SampleRate,
        unsigned short int Channels);

    void NewSampleRate (unsigned int NewRate);
    void NewNumberOfChannels (unsigned short int NewChannels);

    virtual void WriteAudio (Signal& AudioData)
        throw (AudioWriterException);
};

#endif

//************************** ALSAWAVEWRITER.HPP *****************************
