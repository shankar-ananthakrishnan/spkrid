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
// WAVEREADER.CPP                                                           *
//                                                                          *
// Implementation of the WaveReader class, for reading WAVE audio files.    *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on January 26, 2002.                                       *
//                                                                          *
//***************************************************************************

#include "../../signal/signal.hpp"
#include "../audioreader.hpp"
#include "wavereader.hpp"

//***************************************************************************
WaveReader::WaveReader (char *Source) : AudioReader (Source) {}

//***************************************************************************
unsigned short int WaveReader::ChannelCount (void) {
    return (NChannels);                 // 1 = mono, 2 = stereo
}

//****************************************************************************
unsigned short int WaveReader::AudioFormat (void) {
    return (NCompression);              // compression scheme
}

//***************************** WAVEREADER.CPP ******************************
