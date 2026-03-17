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
// WAVEWRITER.CPP                                                           *
//                                                                          *
// Implementation of the WaveWriter class, for writing WAVE audio files.    *
// Helper component for the Speaker Identification Project.                 *
//                                                                          *
// Last modified on January 26, 2002.                                       *
//                                                                          *
//***************************************************************************

#include "../../signal/signal.hpp"
#include "../audiowriter.hpp"
#include "wavewriter.hpp"

//***************************************************************************
WaveWriter::WaveWriter (char *Target, unsigned int SRate,
    unsigned short int Res,
    unsigned short int Channels) : AudioWriter (Target, SRate, Res) {
    NChannels = Channels;
}

//***************************************************************************
unsigned short int WaveWriter::ChannelCount (void) {
    return (NChannels);                 // 1 = mono, 2 = stereo
}

//***************************** WAVEWRITER.CPP ******************************
