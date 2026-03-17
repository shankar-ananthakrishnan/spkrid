//***************************************************************************
//                                                                          *
// WINWAVEWRITER.CPP                                                        *
//                                                                          *
// Implementation of `WinWaveWriter', a class that outputs audio via the    *
// Windows device driver. Uses Win32 API calls. Nonportable class.          *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on March 16, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <windows.h>
#include <mmsystem.h>

#include "../../signal/signal.hpp"
#include "../audiowriter.hpp"
#include "winwavewriter.hpp"

#define    NBUFFERS         2           // use two buffers
#define    BUFFER_SIZE    10000         // number of samples / buffer

//***************************************************************************
WinWaveWriter::WinWaveWriter (unsigned int SampleRate,
    unsigned short int Channels) : AudioWriter("Win32 WAVE Audio",
    SampleRate, 16), NChannels(Channels) {}     // set playback parameters

//***************************************************************************
void WinWaveWriter::NewSampleRate (unsigned int NewRate) {
    NResolution = NewRate;
}

//***************************************************************************
void WinWaveWriter::NewNumberOfChannels (unsigned short int NewChannels) {
    NChannels = NewChannels;
}

//***************************************************************************
void WinWaveWriter::WriteAudio (Signal& AudioData)
    throw (AudioWriterException) {
    HANDLE BufferDone;                  // event signal buffer done
    HWAVEOUT AudioHandle;               // handle identifying audio device
    WAVEFORMATEX WaveFmt;               // wave audio format information
    WAVEHDR WaveBuf[NBUFFERS];
    MMRESULT AudioResult;
    unsigned int SamplesDone;           // tracks # of samples written
    unsigned int Index;
    unsigned int SampNo;
    signed short int *SampleData;       // pointer for buffer data

    BufferDone = CreateEvent (NULL, TRUE, FALSE, NULL); // get event handle

    WaveFmt.wFormatTag = WAVE_FORMAT_PCM;       // set up WAVE audio params
    WaveFmt.nChannels = NChannels;
    WaveFmt.nSamplesPerSec = NSampleRate;
    WaveFmt.wBitsPerSample = NResolution;
    WaveFmt.nBlockAlign = WaveFmt.nChannels * (WaveFmt.wBitsPerSample / 8);
    WaveFmt.nAvgBytesPerSec = WaveFmt.nSamplesPerSec * WaveFmt.nBlockAlign;
    WaveFmt.cbSize = 0;
                                        // try to open audio device for write
    AudioResult = waveOutOpen (&AudioHandle, WAVE_MAPPER,
        (LPWAVEFORMATEX)&WaveFmt, (DWORD)BufferDone, 0, CALLBACK_EVENT);

    if (AudioResult != MMSYSERR_NOERROR) {      // couldn't open device ?
        CloseHandle (BufferDone);
        throw (AWX_NONEXISTENT);
    }

// We will be using NBUFFERS cyclic buffers to play back the audio data.
// Allocate the buffers and prepare them for use.

    for (Index = 0; Index < NBUFFERS; ++Index) {
        WaveBuf[Index].lpData =
            (LPBYTE)new unsigned char [BUFFER_SIZE * NResolution / 8];
        WaveBuf[Index].dwBufferLength = BUFFER_SIZE * NResolution / 8;
        WaveBuf[Index].dwBytesRecorded = 0;
        WaveBuf[Index].dwUser = 0;
        WaveBuf[Index].dwFlags = 0;
        WaveBuf[Index].dwLoops = 0;

        waveOutPrepareHeader (AudioHandle,
            (LPWAVEHDR)&WaveBuf[Index], sizeof(WAVEHDR));
    }

    SamplesDone = Index = 0;

    while (SamplesDone < AudioData.SampleCount ()) {
        ResetEvent (BufferDone);        // event initially unsignalled
                                        // fill buffer with sample data
        SampleData = (signed short int *)WaveBuf[Index].lpData;
        for (SampNo = 0; SampNo < BUFFER_SIZE; ++SampNo) {
            SampleData[SampNo] = AudioData[SamplesDone + SampNo] * 32768.0;
        }
                                        // write buffer out to device
        waveOutWrite (AudioHandle, (LPWAVEHDR)&WaveBuf[Index],
            sizeof(WAVEHDR));

        if (++Index == NBUFFERS) Index = 0;     // switch to next buffer
        SamplesDone += BUFFER_SIZE;     // track how much audio output

        WaitForSingleObject (BufferDone, INFINITE);     // wait for signal
    }

    waveOutReset (AudioHandle);         // stop playback

    for (Index = 0; Index < NBUFFERS; ++Index) {    // get rid of buffers
        waveOutUnprepareHeader (AudioHandle,
            (LPWAVEHDR)&WaveBuf[Index], sizeof(WAVEHDR));
        delete[] WaveBuf[Index].lpData;
    }

    waveOutClose (AudioHandle);         // done with device
    CloseHandle (BufferDone);           //   and event handle
}

//*************************** WINWAVEWRITER.CPP *****************************
