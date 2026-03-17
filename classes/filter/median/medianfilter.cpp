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
// MEDIANFILTER.CPP                                                         *
//                                                                          *
// Implementation of class `MedianFilter', which implements an N-length     *
// equal weight median filter. Derived from the generic base `Filter'.      *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on April 20, 2002.                                         *
//                                                                          *
//***************************************************************************

#include "../../signal/signal.hpp"
#include "medianfilter.hpp"

//***************************************************************************
MedianFilter::MedianFilter (unsigned int Length) {
    FilterLength = Length + (1 - (Length % 2));     // force odd length
}

//***************************************************************************
void MedianFilter::ApplyFilter (Signal& ISignal, Signal& OSignal) {
    float *MedianWindow;                // window sample shift register
    float *WorkBuffer;                  // sort buffer
    float Temp;
    unsigned int Index;                 // (Index)th element of input
    unsigned int I;
    bool Swapped;
                                        // create buffers
    MedianWindow = new float [FilterLength];
    WorkBuffer = new float [FilterLength];

    ISignal.SetReadMode ();             // make sure this is OK
    OSignal.SetWriteMode ();
                                        // populate median window with
    for (Index = 0; Index < FilterLength; ++Index) {    // first set of data
        MedianWindow[Index] = ISignal[Index];
    }

// This median filter implementation works with fully overlapping windows.
// If N = sample data length, M = median filter length, then the number of
// samples appearing at the output is not N + M - 1 as for a conventional FIR,
// but N - M samples. Thus for proper filtering, N >> M.

    for (Index = FilterLength; Index < ISignal.SampleCount (); ++Index) {
        for (I = 0; I < FilterLength - 1; ++I) {
            WorkBuffer[I] = MedianWindow[I];        // copy window to buffer
            MedianWindow[I] = MedianWindow[I + 1];  // shift window left
        }

        WorkBuffer[I] = MedianWindow[I - 1];    // get last sample in buffer
        MedianWindow[I] = ISignal[Index];       // push in a new sample

        do {                            // BubbleSort
            Swapped = false;

            for (I = 0; I < FilterLength - 1; ++I) {
                if (WorkBuffer[I] > WorkBuffer[I + 1]) {
                    Temp = WorkBuffer[I];
                    WorkBuffer[I] = WorkBuffer[I + 1];
                    WorkBuffer[I + 1] = Temp;
                    Swapped = true;
                }
            }
        } while (Swapped);

        OSignal.AddSample (WorkBuffer[FilterLength / 2]);
    }

    OSignal.SetReadMode ();

    delete[] MedianWindow;
    delete[] WorkBuffer;
}

//************************** MEDIANFILTER.CPP *******************************
