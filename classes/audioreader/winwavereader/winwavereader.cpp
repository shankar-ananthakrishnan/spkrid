//***************************************************************************
//                                                                          *
// SPKRID, A Prototype Text-Dependent Speaker Identification System.        *
// Copyright (C) 2002 A.Sankaranarayanan.                                   *
//                                                                          *
// This file is part of SPKRID.                                             *
//                                                                          *
// SPKRID is free software; you can redistribute it and/or                  *
// modify it under the terms of the GNU General Public License              *
// as published by the Free Software Foundation; either version 2           *
// of the License, or (at your option) any later version.                   *
//                                                                          *
// This program is distributed in the hope that it will be useful,          *
// but WITHOUT ANY WARRANTY; without even the implied warranty of           *
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
// GNU General Public License for more details.                             *
//                                                                          *
// You should have received a copy of the GNU General Public License        *
// along with this program; if not, write to the Free Software              *
// Foundation, Inc., 59 Temple Place - Suite 330, Boston,                   *
// MA  02111-1307, USA.                                                     *
//                                                                          *
// My e-mail address is: total_nerd@hotmail.com.                            *
//                                                                          *
//                                                                          *
// WINWAVEREADER.CPP                                                        *
//                                                                          *
// Implementation of `WinWaveReader', a class which reads audio through a   *
// Windows device driver. Uses Win32 API calls. Nonportable class.          *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on March 15, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <windows.h>
#include <mmsystem.h>

#include "winwavereader.hpp"

#define    NBUFFERS         2
#define    BUFFER_SIZE    10000

//***************************************************************************
WinWaveReader::WinWaveReader (unsigned int SampleRate,
    float Length) : AudioReader ("Win32 WAVE Audio") {
    NSampleRate = SampleRate;
    NResolution = 16;                   // assume 16-bit resolution
    AudioLength = Length * SampleRate;  // number of samples to read
}

//***************************************************************************
void WinWaveReader::NewSampleRate (unsigned int NewRate) {
    NSampleRate = NewRate;
}

//***************************************************************************
void WinWaveReader::NewMediaLength (float NewLength) {
    AudioLength = NewLength * NSampleRate;
}

//***************************************************************************
Signal& WinWaveReader::AudioData (void) throw (AudioReaderException) {
    HANDLE BufferDone;                  // event tells us buffer's full
    HWAVEIN AudioHandle;                // to access audio device
    WAVEHDR WaveBuf[NBUFFERS];
    WAVEFORMATEX WaveFmt;
    MMRESULT AudioResult;
    unsigned int SamplesDone;
    unsigned int Index;
    unsigned int SampNo;
    signed short int *SampleData;

    BufferDone = CreateEvent (NULL, TRUE, FALSE, NULL); // get event handle

    WaveFmt.wFormatTag = WAVE_FORMAT_PCM;       // set up WAVE audio params
    WaveFmt.nChannels = 1;
    WaveFmt.nSamplesPerSec = NSampleRate;
    WaveFmt.wBitsPerSample = NResolution;
    WaveFmt.nBlockAlign = WaveFmt.nChannels * (NResolution / 8);
    WaveFmt.nAvgBytesPerSec = NSampleRate * WaveFmt.nBlockAlign;
    WaveFmt.cbSize = 0;
                                        // try to open audio source
    AudioResult = waveInOpen (&AudioHandle, WAVE_MAPPER,
        (LPWAVEFORMATEX)&WaveFmt, (DWORD)BufferDone, 0, CALLBACK_EVENT);

    if (AudioResult != MMSYSERR_NOERROR) {  // couldn't be opened ?
        CloseHandle (BufferDone);
        throw (ARX_NONEXISTENT);
    }
    
// Since we work with multiple buffers, we have to prepare each and add it
// to the buffer queue used by the audio device driver.

    for (Index = 0; Index < NBUFFERS; ++Index) {
        WaveBuf[Index].lpData =
            (LPBYTE)new unsigned char [BUFFER_SIZE * NResolution / 8];
        WaveBuf[Index].dwBufferLength = BUFFER_SIZE * NResolution / 8;
        WaveBuf[Index].dwBytesRecorded = 0;
        WaveBuf[Index].dwUser = 0;
        WaveBuf[Index].dwFlags = 0;
        WaveBuf[Index].dwLoops = 0;

        waveInPrepareHeader (AudioHandle,
            (LPWAVEHDR)&WaveBuf[Index], sizeof(WAVEHDR));

        waveInAddBuffer (AudioHandle,
            (LPWAVEHDR)&WaveBuf[Index], sizeof(WAVEHDR));
    }

// We use the Win32 event signalling mechanism to wake our program when an
// audio buffer is full. Every time an audio buffer is full, the event is
// `signalled', so we reset it to `not signalled' and then save the data to
// a signal object. We then add the buffer back into the queue. This process
// is repeated till the required number of samples is read.

    waveInStart (AudioHandle);          // start recording audio
    ResetEvent (BufferDone);

    SamplesDone = Index = 0;

    while (SamplesDone < AudioLength) {
        WaitForSingleObject (BufferDone, INFINITE);     // wait for signal
        ResetEvent (BufferDone);        // reset event

// Get at the samples through a short int pointer, add the samples to the
// member Signal class after appropriate normalization.

        SampleData = (signed short int *)WaveBuf[Index].lpData;

        for (SampNo = 0; SampNo < BUFFER_SIZE; ++SampNo) {
            AudioSignal->AddSample (SampleData[SampNo] / 32768.0);
        }

        waveInAddBuffer (AudioHandle,
            (LPWAVEHDR)&WaveBuf[Index], sizeof(WAVEHDR));

        SamplesDone += BUFFER_SIZE;
        if (++Index == NBUFFERS) Index = 0;
    }

    waveInReset (AudioHandle);          // stop recording

    for (Index = 0; Index < NBUFFERS; ++Index) {    // get rid of buffers
        waveInUnprepareHeader (AudioHandle,
            (LPWAVEHDR)&WaveBuf[Index], sizeof(WAVEHDR));
        delete[] WaveBuf[Index].lpData;
    }

    waveInClose (AudioHandle);          // close audio device
    CloseHandle (BufferDone);           // destroy event handle

    AudioSignal->SetReadMode ();        // set up so client can read
    return (*AudioSignal);
}

//*************************** WINWAVEREADER.CPP *****************************
