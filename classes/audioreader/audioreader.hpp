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
// AUDIOREADER.HPP                                                          *
//                                                                          *
// Header file for the AudioReader class, an abstract base class interface  *
// for reading audio sources (files, audio cards, etc.).                    *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on January 26, 2002.                                       *
//                                                                          *
//***************************************************************************

#ifndef __AUDIOREADER_H
#define __AUDIOREADER_H

#include "../signal/signal.hpp"

enum AudioReaderException {             // types of exceptions that can
    ARX_INVALID_FORMAT,                 //   occur when reading audio sources
    ARX_NONEXISTENT
};

//***************************************************************************
class AudioReader {
private:
    char *SAudioSource;                 // pointer to audio source

protected:
    unsigned int NSampleRate;           // samples per second
    unsigned short int NResolution;     // quantiser resolution
    Signal *AudioSignal;                // holds audio data

public:
    AudioReader (char *Source);         // constructor
    ~AudioReader ();

    char *AudioSource (void);           // returns ptr to audio source name

    unsigned int SampleRate (void);     // get sampling rate (Hz)
    unsigned short int Resolution (void);   // and quantizer resolution

    virtual Signal& AudioData (void) = 0;   // implemented by subclass
};
    
#endif

//***************************** AUDIOREADER.HPP *****************************
