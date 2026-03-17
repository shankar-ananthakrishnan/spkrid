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
// WAVEWRITER.HPP                                                           *
//                                                                          *
// Header file for the WaveWriter class, for writing WAVE audio files.      *
// Also an abstract base class, since many different formats exist.         *
// Helper component for the Speaker Identification Project.                 *
//                                                                          *
// Last modified on January 26, 2002.                                       *
//                                                                          *
//***************************************************************************

#ifndef __WAVEWRITER_H
#define __WAVEWRITER_H

#include "../../signal/signal.hpp"
#include "../audiowriter.hpp"

#define    RIFF_CHUNK    0x46464952     // IDs for different chunks */
#define    WAVE_TYPE     0x45564157
#define    FRMT_CHUNK    0x20746D66 
#define    DATA_CHUNK    0x61746164

#define    PCM_UNCOMPRESSED    0x0001   // various formats for WAVE audio
#define    MICROSOFT_ADPCM     0x0002
#define    ITU_G711_A_LAW      0x0006
#define    ITU_G711_U_LAW      0x0007
#define    IMA_ADPCM           0x0011
#define    ITU_G723_ADPCM      0x0016
#define    GSM_610             0x0031
#define    ITU_G721_ADPCM      0x0040
#define    MPEG                0x0050

#define    EVEN(x)     ((x % 2) == 0)   // true if `x' is word-aligned

//***************************************************************************
class WaveWriter : public AudioWriter {
private:
    unsigned short int NChannels;       // mono / stereo

public:
    WaveWriter (char *Target, unsigned int SRate,
        unsigned short int Res, unsigned short int Channels);
                                        
    unsigned short int ChannelCount (void);     // gets number of channels
                                        
    virtual void WriteAudio (Signal& AudioData)
        throw (AudioWriterException) = 0;       // implemented by subclass
};
    
#endif

//***************************** WAVEWRITER.HPP ******************************
