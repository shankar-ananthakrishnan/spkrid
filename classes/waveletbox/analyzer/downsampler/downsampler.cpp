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
// DOWNSAMPLER.CPP                                                          *
//                                                                          *
// Implementation of class `DownSampler', which samples down a signal by a  *
// factor of two and retains either odd or even samples.                    *
// Helper class for implementors of `WaveletAnalyzer'.                      *
//                                                                          *
// Last modified on February 8, 2002.                                       *
//                                                                          *
//***************************************************************************

#include "../../../signal/signal.hpp"
#include "downsampler.hpp"

//***************************************************************************
DownSampler::DownSampler
        (DSamplerType WhichType) : DSType(WhichType) {}

//***************************************************************************
void DownSampler::SampleDown (Signal& InSignal,
        Signal& OutSignal) {
    unsigned int Index;

    Index = (DSType == DNS_EVEN_SAMPLER) ? 0:1;     // starting point
    InSignal.SetReadMode ();            // ensure signal modes are correct
    OutSignal.SetWriteMode ();
                                        // run through source, pick one/two
    for (; Index < InSignal.SampleCount (); Index += 2) {
        OutSignal.AddSample (InSignal[Index]);
    }

    OutSignal.SetReadMode ();           // set up to read
}

//*************************** DOWNSAMPLER.CPP *******************************
