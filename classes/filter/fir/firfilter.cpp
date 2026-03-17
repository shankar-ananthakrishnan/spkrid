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
// FIRFILTER.CPP                                                            *
//                                                                          *
// Implementation of the `FIR_Filter' class, realizes FIR digital filters.  *
// Fundamental component of the Speaker Identification project.             *
//                                                                          *
// Last modified on February 8, 2002.                                       *
//                                                                          *
//***************************************************************************

#include "../../signal/signal.hpp"
#include "../filter.hpp"
#include "firfilter.hpp"

//***************************************************************************
// The constructor copies filter coefft. information from the parameters    *
// passed to private variables, after creating storage for them.            *
//***************************************************************************
FIR_Filter::FIR_Filter (float FFCoeff[], unsigned int NOrder) {
    unsigned int Index;

    Order = NOrder;                     // store filter order

    B = new float [Order];              // copy coefficients
    for (Index = 0; Index < Order; ++Index) {
        B[Index] = FFCoeff[Index];
    }
}

//***************************************************************************
FIR_Filter::~FIR_Filter () {
    delete[] B;                         // free up all storage
}

//***************************************************************************
// `ApplyFilter' implements the FIR digital filter in direct form using the *
// coefficients stored in the array.                                        *
//***************************************************************************
void FIR_Filter::ApplyFilter (Signal& ISignal, Signal& OSignal) {
    unsigned int Index;                 // current sample index
    unsigned int RegIndex;
    float *SReg;                        // intermediate shift register
    float OutSample;                    // filtered sample

    ISignal.SetReadMode ();             // make sure signal mode is correct
    OSignal.SetWriteMode ();

    SReg = new float [Order];           // allocate shift reg
    for (Index = 0; Index < Order; ++Index) {
        SReg[Index] = 0.0;              // stuff with 0's
    }

    for (Index = 0; Index < (ISignal.SampleCount () + Order - 1); ++Index) {
        SReg[Order - 1] = ISignal[Index];       // read next sample
        OutSample = 0.0;

        for (RegIndex = 0; RegIndex < Order; ++RegIndex) {
            OutSample += B[RegIndex] * SReg[Order - RegIndex - 1];
        }

        for (RegIndex = 0; RegIndex < (Order - 1); ++RegIndex) {
            SReg[RegIndex] = SReg[RegIndex + 1];
        }

// At this point, `OutSample' contains the filtered version of the Index(th)
// input data point. Add this sample to the output signal.

        OSignal.AddSample (OutSample);
    }

    OSignal.SetReadMode ();             // set it up so that it can be read
    delete[] SReg;                      // get rid of shift register
}

//*************************** FIRFILTER.CPP *********************************
