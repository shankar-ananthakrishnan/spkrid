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
// SIGNAL.HPP                                                               *
//                                                                          *
// Template for the `Signal' class, which handles discrete signals.         *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on March 29, 2002.                                         *                             
//                                                                          *
//***************************************************************************

#ifndef __SIGNAL_HPP
#define __SIGNAL_HPP                    // protect against double inclusion

#include <stdio.h>

#define  FRAME_SIZE   1000              // length of one frame of samples
#define  SAMPLE_SIZE  sizeof(float)     // size in bytes of a sample

enum SignalException {                  // exception codes
    SGX_BOUNDS_ERROR,                   // thrown if index is invalid
    SGX_INVALID_MODE                    // thrown if mode is incorrect
};

enum SignalAccessMode {                 // access modes for Signal object
    SG_READ_MODE,
    SG_WRITE_MODE
};

class Signal {
private:
    static unsigned int UseCount;       // for creating unique tempfiles
    static unsigned int InUse;          // for resetting UseCount

    float *Sig;                         // buffer for samples
    float *WriteBuffer;                 // write buffer accumulates samples
    unsigned int FrameStart;            // start address for every frame
    unsigned int NSamples;              // total number of samples
    unsigned int WriteBufPtr;           // write buffer pointer
    SignalAccessMode Mode;              // checks read or write mode
    char FileName[20];                  // name of tempfile
    FILE *TempFile;                     // tempfile pointer

public:
    Signal (void);
    ~Signal ();

    void SetReadMode (void);            // sets read / write mode
    void SetWriteMode (void);
    unsigned int SampleCount (void);    // get number of samples in signal
    void Dump (char *DumpFileName);     // dumps signal to file (debugging)

    void AddSample (float Sample)
        throw (SignalException);        // add a sample to tempfile

    float operator[] (unsigned int Index)
        throw (SignalException);        // overloaded [] retrieves a sample

    float Energy (unsigned int Lower, unsigned int Upper)
        throw (SignalException);        // computes energy of signal
};

#endif

//****************************** SIGNAL.HPP *********************************
