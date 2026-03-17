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
// AUDIOWRITER.HPP                                                          *
//                                                                          *
// Header file for the AudioWriter class, an abstract base class interface  *
// for writing audio sources (files, audio cards, etc.).                    *
// Helper component for the Speaker Identification Project.                 *
//                                                                          *
// Last modified on January 26, 2002.                                       *
//                                                                          *
//***************************************************************************

#ifndef __AUDIOWRITER_H
#define __AUDIOWRITER_H

#include "../signal/signal.hpp"

enum AudioWriterException {             // types of exceptions that can
    AWX_NONEXISTENT,                    //   occur when writing audio sources
    AWX_WRITE_ERROR
};

//***************************************************************************
class AudioWriter {
private:
    char *SAudioTarget;                 // pointer to audio source

protected:
    unsigned int NSampleRate;           // samples per second
    unsigned short int NResolution;     // quantiser resolution

public:
    AudioWriter (char *Target, unsigned int SRate,
        unsigned short int Resolution); // constructor

    ~AudioWriter ();

    char *AudioTarget (void);               // ptr to audio source name
    unsigned int SampleRate (void);         // sampling rate in Hz
    unsigned short int Resolution (void);   // resolution in bits/sample
                                        
    virtual void WriteAudio (Signal& AudioData) 
        throw (AudioWriterException) = 0;   // implemented by subclasses
};
    
#endif

//***************************** AUDIOWRITER.HPP *****************************
