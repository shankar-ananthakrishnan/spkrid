//***************************************************************************
//                                                                          *
// WINWAVEWRITER.HPP                                                        *
//                                                                          *
// Header file for `WinWaveWriter', a class that outputs audio via the      *
// Windows device driver. Uses Win32 API calls. Nonportable class.          *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on March 16, 2002.                                         *
//                                                                          *
//***************************************************************************

#ifndef __WINWAVEWRITER_HPP
#define __WINWAVEWRITER_HPP

#include "../../signal/signal.hpp"
#include "../audiowriter.hpp"

//***************************************************************************
class WinWaveWriter : public AudioWriter {
private:
    unsigned short int NChannels;

public:
    WinWaveWriter (unsigned int SampleRate,
        unsigned short int Channels);

    void NewSampleRate (unsigned int NewRate);
    void NewNumberOfChannels (unsigned short int NewChannels);

    virtual void WriteAudio (Signal& AudioData)
        throw (AudioWriterException);
};

#endif

//*************************** WINWAVEWRITER.HPP *****************************
