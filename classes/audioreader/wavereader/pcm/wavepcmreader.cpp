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
// WAVEPCMREADER.CPP                                                        *
//                                                                          *
// Implementation of the WavePCMReader class, for reading WAVE audio        *
// which is in the standard PCM/Uncompressed format.                        *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on January 26, 2002.                                       *
//                                                                          *
//***************************************************************************

#include <stdio.h>

#include "../../../signal/signal.hpp"
#include "../../audioreader.hpp"
#include "../wavereader.hpp"
#include "wavepcmreader.hpp"

//***************************************************************************
WavePCMReader::WavePCMReader (char *Source) : WaveReader (Source) {
    ReadPCMAudio ();                    // call helper to read source
}

//***************************************************************************
Signal& WavePCMReader::AudioData (void) {
    return (*AudioSignal);              // toss reference to AudioSignal
}

//***************************************************************************
// `ReadPCMAudio' reads the WAVE audio file, stores samples and format data *
// in the class hierarchy. If the file is not openable, or if the format is *
// incorrect, an exception is thrown.                                       *
//***************************************************************************
void WavePCMReader::ReadPCMAudio (void)
            throw (AudioReaderException) {
    FILE *WaveFilePtr;                  // pointer to wave file
    long int FileSize;
    unsigned int ChunkID, ChunkSize;    // wave chunk id, size
    unsigned int DataChunkSize;         // size of DATA chunk
    unsigned long int DataAddress;      // address in file of DATA chunk
    unsigned short int ExtraFRMT;       // extra bytes in FORMAT chunk
    unsigned char Sample_8Bit;          // samples may be 8-bit
    short int Sample_16Bit;             //   or 16-bit
    unsigned int Index;
    bool InvalidFormat = false;         // true if file format incorrect
    
    if ((WaveFilePtr = fopen (AudioSource (), "rb")) == NULL) {
        throw (ARX_NONEXISTENT);
    }

    fseek (WaveFilePtr, 0, SEEK_END);   // read size of wave file
    FileSize = ftell (WaveFilePtr);
    rewind (WaveFilePtr);

    fread (&ChunkID, 4, 1, WaveFilePtr);

                                        // check first chunk for RIFF id
    if (ChunkID != RIFF_CHUNK) InvalidFormat = true;

    fseek (WaveFilePtr, 4, SEEK_CUR);       // skip over RIFF chunk size
    fread (&ChunkID, 4, 1, WaveFilePtr);    // read next id dword

                                        // check for WAVE signature
    if (ChunkID != WAVE_TYPE) InvalidFormat = true;

    fclose (WaveFilePtr);               // prepare for exception

    if (InvalidFormat) throw (ARX_INVALID_FORMAT);

    WaveFilePtr = fopen (AudioSource (), "rb");   // reopen, get back
    fseek (WaveFilePtr, 12, SEEK_SET);      //   to where we left off

// The chunks in the wave file may be in any order, so we have to scan the
// entire file. In this method, we ignore all chunks except the FORMAT and
// DATA chunks. These chunks may occur in any order, so if we encounter the
// DATA chunk before the format chunk, we record its size and location. We
// then continue our search for the FORMAT chunk; only when we find this
// chunk can we interpret the contents of the DATA chunk. If any other chunk
// is found, we simply skip over it. After processing the entire file, we
// revisit the DATA chunk and read the data into a Signal object.

    while (ftell (WaveFilePtr) < FileSize) {
                                        // read next chunk ID
        fread (&ChunkID, 4, 1, WaveFilePtr);

        switch (ChunkID) {
            case FRMT_CHUNK:            // FORMAT chunk, read parameters
                fread (&ChunkSize, 4, 1, WaveFilePtr);
                fread (&NCompression, 2, 1, WaveFilePtr);

                                        // our routine handles only PCM
                if (NCompression != PCM_UNCOMPRESSED) {
                    fclose (WaveFilePtr);
                    throw (ARX_INVALID_FORMAT);
                }

                fread (&NChannels, 2, 1, WaveFilePtr);
                fread (&NSampleRate, 4, 1, WaveFilePtr);
                fseek (WaveFilePtr, 4, SEEK_CUR);
                fseek (WaveFilePtr, 2, SEEK_CUR);
                fread (&NResolution, 2, 1, WaveFilePtr);

                (ChunkSize == 0x10) ?
                    ExtraFRMT = 0 : fread (&ExtraFRMT, 2, 1, WaveFilePtr);
                
                fseek (WaveFilePtr, ExtraFRMT +
                         ((EVEN(ExtraFRMT)) ? 0:1), SEEK_CUR);
                break;

            case DATA_CHUNK:            // DATA chunk, record location & size
                fread (&DataChunkSize, 4, 1, WaveFilePtr);
                DataAddress = ftell (WaveFilePtr);

                fseek (WaveFilePtr, DataChunkSize +
                         ((EVEN(DataChunkSize)) ? 0:1), SEEK_CUR);
                break;

            default:                    // some other chunk, ignore
                fread (&ChunkSize, 4, 1, WaveFilePtr);
                fseek (WaveFilePtr, ChunkSize +
                         ((EVEN(ChunkSize)) ? 0:1), SEEK_CUR);
                break;
        }
    }
                                        // revisit the DATA chunk
    fseek (WaveFilePtr, DataAddress, SEEK_SET);

// For 8-bit sampling in the wave format, the zero level is 0x80. For files
// in this format, therefore, we must apply a correction by subtracting this
// value to get it to the zero level. For 16-bit sampling, the samples are
// stored in two's complement, so they can be directly read. The samples are
// normalized after they are read, so that the peak values do not cross +/-1.
// This is done to facilitate handling of sample data, whether in 8- or
// 16-bit samples.

    switch (NResolution) {              // check resolution
        case 8:                         // 8 bits/sample
            for (Index = 0; Index < DataChunkSize; ++Index) {
                fread (&Sample_8Bit, 1, 1, WaveFilePtr);
                AudioSignal->AddSample ((Sample_8Bit - 0x80) / 128.0);
            }

            break;

        case 16:                        // 16 bits/sample
            for (Index = 0; Index < (DataChunkSize / 2); ++Index) {
                fread (&Sample_16Bit, 2, 1, WaveFilePtr);
                AudioSignal->AddSample (Sample_16Bit / 32768.0);
            }

            break;
    }

    AudioSignal->SetReadMode ();        // set it up to read

    fclose (WaveFilePtr);               // close the wave file
}


//**************************** WAVEPCMREADER.CPP ****************************
