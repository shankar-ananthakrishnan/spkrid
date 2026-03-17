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
// SIGNAL.CPP                                                               *
//                                                                          *
// Implementation of the `Signal' class, which handles discrete signals.    *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on March 29, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "signal.hpp"

unsigned int Signal::UseCount = 0;      // reset class object count
unsigned int Signal::InUse = 0;

//***************************************************************************
// The constructor performs the following functions:                        *
//                                                                          *
// 1. Allocates memory for one frame of the discrete signal.                *
// 2. Creates a tempfile combining "temp." and the object count.            *
// 3. Sets the access mode to SG_WRITE_MODE.                                *
// 4. Sets the initial frame value.                                         *
//***************************************************************************
Signal::Signal (void) {
    char Number[10];                    // buffer for converted number

    Sig = new float [FRAME_SIZE];       // space for one frame of samples
    WriteBuffer = new float [FRAME_SIZE];   // allocate write buffer
    itoa (UseCount, Number, 10);        // convert object count to string
    strcpy (FileName, "temp.");         // concat. with "temp." to create
    strcat (FileName, Number);          //   a unique tempfile for every
    TempFile = fopen (FileName, "wb");  //   discrete signal
    NSamples = 0;                       // no samples initially
    Mode = SG_WRITE_MODE;               // initial access mode SG_WRITE_MODE
    FrameStart = 0;                     // first frame is in memory
    WriteBufPtr = 0;                    // write buffer beginning
    ++UseCount;                         // bump object count
    ++InUse;                            // one more signal in use
}

//***************************************************************************
// The destructor performs the following cleanup activities:                *
//                                                                          *
// 1. De-allocates the memory allocated for the sample frame.               *
// 2. Closes the tempfile and deletes it.                                   *
//***************************************************************************
Signal::~Signal () {
    delete[] Sig;                       // deallocate memory
    delete[] WriteBuffer;               // destroy write buffer
    fclose (TempFile);                  // close file
    remove (FileName);                  //   and delete it

    if (--InUse == 0) UseCount = 0;     // reset UseCount if no signals used
}

//***************************************************************************
// `SampleCount' returns the number of samples currently in the signal.     *
// It simply returns the value of the NSamples private variable.            *
//***************************************************************************
unsigned int Signal::SampleCount (void) {
    return (NSamples);                  // throw number of samples at caller
}

//***************************************************************************
// `SetReadMode' sets the access mode to SG_READ_MODE. It closes the file & *
// opens it again in read mode, resetting the file pointer to the start.    *
//***************************************************************************
void Signal::SetReadMode (void) {
                                        // flush unwritten data to disk
    fwrite (WriteBuffer, SAMPLE_SIZE, WriteBufPtr, TempFile);
    WriteBufPtr = 0;
    fclose (TempFile);                  // close & reopen file
    TempFile = fopen (FileName, "rb");
    Mode = SG_READ_MODE;
}

//***************************************************************************
// `SetWriteMode' sets the access mode to SG_WRITE_MODE. It closes the file *
// and opens it again in write mode, setting the file pointer to the end.   *
// Samples are always added at the end of the file.                         *
//***************************************************************************
void Signal::SetWriteMode (void) {
    fclose (TempFile);                  // close & reopen file
    TempFile = fopen (FileName, "wb");
    fseek (TempFile, 0, SEEK_END);      // go to end of file
    Mode = SG_WRITE_MODE;
}

//***************************************************************************
// `AddSample' adds a sample to the tempfile at the current location of the *
// file pointer (normally set to the end). If access mode is SG_READ_MODE,  *
// SignalException `SGX_INVALID_MODE' is thrown. The first frame of samples *
// is also loaded into the memory buffer. `AddSample' buffers the write     *
// operation by accumulating samples in `WriteBuffer' till one full frame   *
// has been written; it then flushes this buffer to disk in one write oper- *
// ation. This technique greatly speeds up swapping and decreases overhead. *
//***************************************************************************
void Signal::AddSample (float Sample) throw (SignalException) {
    if (Mode == SG_READ_MODE) throw (SGX_INVALID_MODE);

    if (NSamples < FRAME_SIZE) {        // load first frame in buffer
        Sig[NSamples] = Sample;
    }
                                        // accumulate samples in WriteBuffer
    if (WriteBufPtr < FRAME_SIZE) {
        WriteBuffer[WriteBufPtr++] = Sample;
    }
                                        // if WriteBuffer is full, flush
    if (WriteBufPtr == FRAME_SIZE) {
        fwrite (WriteBuffer, SAMPLE_SIZE, FRAME_SIZE, TempFile);
        WriteBufPtr = 0;
    }

    ++NSamples;                         // another sample to total
}

//***************************************************************************
// The `[]' operator is overloaded to make the signal look like an array of *
// floats. It first checks if the access mode is SG_READ_MODE; if not,      *
// SignalException `SGX_INVALID_MODE' is thrown. If the index passed is not *
// within bounds, SGX_BOUNDS_ERROR is thrown. If the given element is not   *
// in the current frame, a new frame is loaded from the tempfile into the   *
// memory buffer.                                                           *
//***************************************************************************
float Signal::operator [] (unsigned int Index) throw (SignalException) {
    unsigned int Frame;                 // frame #
    unsigned int Size;                  // number of samples in frame
                                        
    if (Mode == SG_WRITE_MODE)          // check access mode
        throw (SGX_INVALID_MODE);
                                        
    if (Index >= NSamples) {            // check Index bounds
#ifdef THROW_BOUNDS_EXCEPTION           // if defined, throw exception
        throw (SGX_BOUNDS_ERROR);
#else
        return (0.0);                   // if not, return 0.0
#endif
    }
                                        // if not in current frame
    if (Index >= (FrameStart + FRAME_SIZE) ||
             (Index < FrameStart)) {
        Frame = Index / FRAME_SIZE;     // compute frame #
        fseek (TempFile, Frame * FRAME_SIZE * SAMPLE_SIZE, SEEK_SET);

        Size = FRAME_SIZE;              // final frame adjustment
        if (Frame == (NSamples / FRAME_SIZE)) {
            Size = NSamples % FRAME_SIZE;
        }
                                        // read samples from file to buffer
        fread (Sig, SAMPLE_SIZE, Size, TempFile);
                                        // change frame beginning pointer
        FrameStart = Frame * FRAME_SIZE;
    }

    return (Sig[Index - FrameStart]);   // return the Index(th) element
}

//***************************************************************************
// The `Energy' method computes the energy of the signal within the limits  *
// specified by `Lower' and `Upper'. The minimum value for `Lower' is 0;    *
// the upper bound must be less than NSamples. Further, `Lower' < `Upper'   *
//***************************************************************************
float Signal::Energy (unsigned int Lower, unsigned int Upper)
                    throw (SignalException) {
    unsigned int Index;
    float Sample;
    float SEnergy;                      // partial energy accumulator

                                        // check if indices are within bounds
    if (Lower > Upper) {
        throw (SGX_BOUNDS_ERROR);       // if not, throw exception
    }
                                        // compute energy
    for (Index = Lower, SEnergy = 0.0; Index <= Upper; ++Index) {
        Sample = (*this)[Index];
        SEnergy += Sample * Sample;
    }

    return (SEnergy);                   // and toss it back
}

//***************************************************************************
// `Dump' writes the samples in the current signal to a dump file in ASCII  *
// mode. It assumes that the signal is in read mode. This method is often   *
// useful for debugging purposes.                                           *
//***************************************************************************
void Signal::Dump (char *DumpFileName) {
    FILE *DumpFile;
    unsigned int Index;

    DumpFile = fopen (DumpFileName, "wt");

    for (Index = 0; Index < SampleCount (); ++Index) {      // write to file
        fprintf (DumpFile, "%10.10f\n", (*this)[Index]);
    }

    fclose (DumpFile);
}

//****************************** SIGNAL.CPP *********************************
