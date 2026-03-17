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
// WAVEPCMWRITER.CPP                                                        *
//                                                                          *
// Implementation of the WavePCMWriter class, for writing WAVE audio        *
// files in the standard PCM/Uncompressed format.                           *
// Helper component for the Speaker Identification Project.                 *
//                                                                          *
// Last modified on January 26, 2002.                                       *
//                                                                          *
//***************************************************************************

#include <stdio.h>

#include "../../../signal/signal.hpp"
#include "../../audiowriter.hpp"
#include "../wavewriter.hpp"
#include "wavepcmwriter.hpp"

//***************************************************************************
inline int Round (float FNum) {         // helper function to round float
    int INum = (int)FNum;               //   values up or down

    if ((FNum - INum) >= 0.50) {
        return (INum + 1);
    } else if ((FNum - INum) <= -0.50) {
        return (INum - 1);
    } else {
        return (INum);
    }
}

//***************************************************************************
WavePCMWriter::WavePCMWriter (char *Target, unsigned int SRate,
    unsigned short int Res,
    unsigned short int Chann) : WaveWriter (Target, SRate, Res, Chann) {}

//***************************************************************************
// `WriteAudio' opens the WAVE file, and writes the data in `AudioData' to  *
// the file in PCM/Uncompressed format. Audio data is expected to be norm-  *
// alized with respect to the maximum possible sample value, and stripped   *
// of any DC component which is present in 8-bit WAVE PCM audio. The only   *
// chunks written by `WriteAudio' are the FACT and DATA chunks. For stereo  *
// the samples in `AudioData' are expected to be interleaved.               *
//***************************************************************************
void WavePCMWriter::WriteAudio (Signal& AudioData)
                        throw (AudioWriterException) {
    FILE *WaveFilePtr;                  // pointer to wave file
    unsigned int Index;
    unsigned char Sample_8Bit;          // we may need 8-bit
    short int Sample_16Bit;             //   or 16-bit samples
    unsigned int ChunkType;
    unsigned int ChunkSize;
    unsigned int DataChunkSize;
    unsigned int NSamples;              // samples to write
    unsigned int SampRate;
    unsigned short int AudioFormat;     // WAVE audio compression code
    unsigned short int NChannels;       // 1 = mono, 2 = stereo
    unsigned int BytesPSecond;
    unsigned short int BytesPSample;
    unsigned short int BitsPSample;     // quantizer resolution (bits/sample)

    if ((WaveFilePtr = fopen (AudioTarget (), "wb")) == NULL) {
        throw (AWX_WRITE_ERROR);
    }

    AudioData.SetReadMode ();           // set mode and get no. of samples
    NSamples = AudioData.SampleCount ();

    DataChunkSize = NSamples * (Resolution () / 8);

    ChunkType = RIFF_CHUNK;             // write RIFF chunk info
    ChunkSize = 4 + 8 + 16 + 8 + DataChunkSize;
    fwrite (&ChunkType, 4, 1, WaveFilePtr);
    fwrite (&ChunkSize, 4, 1, WaveFilePtr);

    ChunkType = WAVE_TYPE;              // write WAVE type id
    fwrite (&ChunkType, 4, 1, WaveFilePtr);

    ChunkType = FRMT_CHUNK;             // write FORMAT chunk
    ChunkSize = 16;                     // no frills, plain PCM
    AudioFormat = PCM_UNCOMPRESSED;
    NChannels = ChannelCount ();
    SampRate = SampleRate ();
    BitsPSample = Resolution ();
    BytesPSample = BitsPSample / 8;
    BytesPSecond = BytesPSample * SampRate;

    fwrite (&ChunkType, 4, 1, WaveFilePtr);
    fwrite (&ChunkSize, 4, 1, WaveFilePtr);
    fwrite (&AudioFormat, 2, 1, WaveFilePtr);
    fwrite (&NChannels, 2, 1, WaveFilePtr);
    fwrite (&SampRate, 4, 1, WaveFilePtr);
    fwrite (&BytesPSecond, 4, 1, WaveFilePtr);
    fwrite (&BytesPSample, 2, 1, WaveFilePtr);
    fwrite (&BitsPSample, 2, 1, WaveFilePtr);

    ChunkType = DATA_CHUNK;             // prepare to write DATA chunk
    ChunkSize = DataChunkSize;
    fwrite (&ChunkType, 4, 1, WaveFilePtr);
    fwrite (&ChunkSize, 4, 1, WaveFilePtr);

    switch (BitsPSample) {              // act differently for 8/16 bit
        case 8:
            for (Index = 0; Index < NSamples; ++Index) {
                Sample_8Bit = (unsigned char)
                    Round ((AudioData[Index] * 128.0) + 0x80);
                fwrite (&Sample_8Bit, 1, 1, WaveFilePtr);
            }

            break;

        case 16:
            for (Index = 0; Index < NSamples; ++Index) {
                Sample_16Bit = (short int)
                    (Round (AudioData[Index] * 32768.0));
                fwrite (&Sample_16Bit, 2, 1, WaveFilePtr);
            }

            break;
    }

    fclose (WaveFilePtr);               // close the wave file
}

//**************************** WAVEPCMWRITER.CPP ****************************
