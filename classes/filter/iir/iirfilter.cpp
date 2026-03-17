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
// IIRFILTER.CPP                                                            *
//                                                                          *
// Implementation of the `IIR_Filter' class, realizes IIR digital filters.  *
// Fundamental component of the Speaker Identification project.             *
//                                                                          *
// Last modified on January 21, 2002.                                       *
//                                                                          *
//***************************************************************************

#include "../../signal/signal.hpp"
#include "../filter.hpp"
#include "iirfilter.hpp"

#define MAX(a,b) (((a) > (b)) ? (a):(b))    // larger and smaller of 2 nums
#define MIN(a,b) (((a) < (b)) ? (a):(b))

//***************************************************************************
// The constructor copies filter coefft. information from the parameters    *
// passed to private variables, after creating storage for them.            *
//***************************************************************************
IIR_Filter::IIR_Filter (float FFCoeff[][2], unsigned int NumOrder,
                float FBCoeff[][2], unsigned int DenOrder, float Gain) {
    unsigned int Index;

    Numer = (NumOrder + 1) / 2;         // number of numerator sections
    Denom = (DenOrder + 1) / 2;         // number of denominator sections

    StageGain = Gain;                   // per-stage gain

    A = new float * [Denom];            // copy denominator coefficients
    for (Index = 0; Index < Denom; ++Index) {
        A[Index] = new float [2];
        A[Index][0] = FBCoeff[Index][0];
        A[Index][1] = FBCoeff[Index][1];
    }

    B = new float * [Numer];            // copy numerator coefficients
    for (Index = 0; Index < Numer; ++Index) {
        B[Index] = new float [2];
        B[Index][0] = FFCoeff[Index][0];
        B[Index][1] = FFCoeff[Index][1];
    }
}

//***************************************************************************
IIR_Filter::~IIR_Filter () {
    unsigned int Index;

    for (Index = 0; Index < Denom; ++Index) {       // free up all storage
        delete[] A[Index];                          //   allocated by the
    }                                               //   constructor
    delete[] A;

    for (Index = 0; Index < Numer; ++Index) {
        delete[] B[Index];
    }
    delete[] B;
}

//***************************************************************************
// `ApplyFilter' implements the IIR digital filter in cascaded 2nd order    *
// sections using the coefficients stored in the arrays.                    *
//***************************************************************************
void IIR_Filter::ApplyFilter (Signal& ISignal, Signal& OSignal) {
    unsigned int Sec;                   // current section
    unsigned int Index;                 // current sample index
    float **SReg;                       // intermediate shift registers
    float OutSample;                    // filtered sample
    float Temp;

    ISignal.SetReadMode ();             // make sure signal mode is correct
    OSignal.SetWriteMode ();
                                        // allocate shift reg., fill with 0s
    SReg = new float * [MAX (Numer, Denom)];
    for (Index = 0; Index < MAX (Numer, Denom); ++Index) {
        SReg[Index] = new float [2];
        SReg[Index][0] = SReg[Index][1] = 0.0;
    }

// The cascade network is evaluated for every input sample. Evaluation
// proceeds from left to right, and is divided in two parts. In the first
// part, all complete second order sections are evaluated. Then, depending
// on the relative difference between the numerator and denominator order,
// either only feedforward or feedback sections are evaluated. The output
// of each stage is coupled to the next, after multiplication with the
// stage gain. The `section gain' method is used so that the intermediate
// products do not balloon to enormous values, as is often the case with
// higher order IIR filters.

    for (Index = 0; Index < ISignal.SampleCount (); ++Index) {
        OutSample = ISignal[Index];     // read next sample of input data

        for (Sec = 0; Sec < MIN (Numer, Denom); ++Sec) {
            OutSample += (A[Sec][0] * SReg[Sec][0]) +       // feedback path
                                (A[Sec][1] * SReg[Sec][1]); //   (poles)
            Temp = OutSample;           // intermediate product

            OutSample += (B[Sec][0] * SReg[Sec][0]) +       // feedfwd path
                                (B[Sec][1] * SReg[Sec][1]); //   (zeros)
            OutSample *= StageGain;     // apply section gain

            SReg[Sec][1] = SReg[Sec][0];    // adjust shift registers
            SReg[Sec][0] = Temp;
        }

        if (Numer > Denom) {            // numerator order higher
            for (; Sec < Numer; ++Sec) {
                Temp = OutSample;       // apply only feedfwd coefficients
                OutSample += (B[Sec][0] * SReg[Sec][0]) +
                                    (B[Sec][1] * SReg[Sec][1]);
                OutSample *= StageGain; // section gain
    
                SReg[Sec][1] = SReg[Sec][0];    // adjust shift regs
                SReg[Sec][0] = Temp;
            }
        }
        else if (Numer < Denom) {       // denominator order higher
            for (; Sec < Denom; ++Sec) {
                                        // apply only feedback coefficients
                OutSample += (A[Sec][0] * SReg[Sec][0]) +
                                    (A[Sec][1] * SReg[Sec][1]);
                Temp = OutSample;
                OutSample *= StageGain; // gain
    
                SReg[Sec][1] = SReg[Sec][0];
                SReg[Sec][0] = Temp;
            }
        }

// At this point, `OutSample' contains the filtered version of the Index(th)
// input data point. Add this sample to the output signal.

        OSignal.AddSample (OutSample);
    }

    OSignal.SetReadMode ();             // set it up so that it can be read

    for (Index = 0; Index < MAX (Numer, Denom); ++Index) {
        delete[] SReg[Index];           // get rid of shift registers
    }

    delete[] SReg;
}

//*************************** IIRFILTER.CPP *********************************
