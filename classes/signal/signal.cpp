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
#include "signal.hpp"

//***************************************************************************
// The constructor starts a new in-memory discrete signal in write mode.     *
//***************************************************************************
Signal::Signal (void) {
    Mode = SG_WRITE_MODE;               // initial access mode SG_WRITE_MODE
}

//***************************************************************************
// The vector member handles cleanup automatically.                         *
//***************************************************************************
Signal::~Signal () {
}

//***************************************************************************
// `SampleCount' returns the number of samples currently in the signal.     *
//***************************************************************************
unsigned int Signal::SampleCount (void) {
    return (static_cast<unsigned int>(Samples.size ()));
}

//***************************************************************************
// `SetReadMode' sets the access mode to SG_READ_MODE.                      *
//***************************************************************************
void Signal::SetReadMode (void) {
    Mode = SG_READ_MODE;
}

//***************************************************************************
// `SetWriteMode' sets the access mode to SG_WRITE_MODE. Samples are always *
// added at the end of the signal.                                          *
//***************************************************************************
void Signal::SetWriteMode (void) {
    Mode = SG_WRITE_MODE;
}

//***************************************************************************
// `AddSample' appends a sample. If access mode is SG_READ_MODE,            *
// SignalException `SGX_INVALID_MODE' is thrown.                            *
//***************************************************************************
void Signal::AddSample (float Sample) throw (SignalException) {
    if (Mode == SG_READ_MODE) throw (SGX_INVALID_MODE);

    Samples.push_back (Sample);
}

//***************************************************************************
// The `[]' operator is overloaded to make the signal look like an array of *
// floats. It first checks if the access mode is SG_READ_MODE; if not,      *
// SignalException `SGX_INVALID_MODE' is thrown. If the index passed is not *
// within bounds, SGX_BOUNDS_ERROR is thrown when THROW_BOUNDS_EXCEPTION is *
// defined; otherwise 0.0 is returned to preserve the original behavior.     *
//***************************************************************************
float Signal::operator [] (unsigned int Index) throw (SignalException) {
    if (Mode == SG_WRITE_MODE)          // check access mode
        throw (SGX_INVALID_MODE);

    if (Index >= Samples.size ()) {     // check Index bounds
#ifdef THROW_BOUNDS_EXCEPTION           // if defined, throw exception
        throw (SGX_BOUNDS_ERROR);
#else
        return (0.0);                   // if not, return 0.0
#endif
    }

    return (Samples[Index]);            // return the Index(th) element
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
