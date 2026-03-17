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
// LPANALYZER.CPP                                                           *
//                                                                          *
// Implementation of the LPAnalyzer class, which takes in a speech signal   *
// and produces a signal containing LP coefficients for speech frames.      *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on April 20, 2002.                                         *
//                                                                          *
//***************************************************************************

#include "../globalconstants.hpp"
#include "../signal/signal.hpp"
#include "../filter/fir/firfilter.hpp"
#include "lpredictor/linearpredictor.hpp"
#include "lpanalyzer.hpp"

//***************************************************************************
LPAnalyzer::LPAnalyzer
    (unsigned int CoeffPerFrame) : NCoeff(CoeffPerFrame) {
    Predict = new LinearPredictor (NCoeff);  // a linear predictor
}

//***************************************************************************
LPAnalyzer::~LPAnalyzer () {
    delete Predict;
}

//***************************************************************************
// `PredictorCoeff' breaks the input signal into chunks of suitable size    *
// and feeds them to the linear predictor one after the other. Predictor    *
// coefficients for each frame are entered into the signal `LPCoeff'.       *
//***************************************************************************
void LPAnalyzer::PredictorCoeff (Signal& Speech,
        Signal& LPCoeff, Signal& AutoCorr) {
    float *FrameBuffer;                 // frame of input signal
    float *FrameLPC;                    // returned coeffs
    float *FrameACR;                    // autocorrelation
    unsigned int Lower;                 // for "chunking"
    unsigned int Upper;
    unsigned int Index;
    Filter *PreEmp;
    Signal *PESpeech;
    static float PreEmpHPF[2] = {0.5, -0.49};

    Lower = 0; Upper = MILLISECOND(20);
    FrameBuffer = new float [Upper - Lower];
    FrameLPC = new float [NCoeff];
    FrameACR = new float [NCoeff + 1];

    PESpeech = new Signal;
    PreEmp = new FIR_Filter (PreEmpHPF, 2);
    PreEmp->ApplyFilter (Speech, *PESpeech);
    delete PreEmp;

    LPCoeff.SetWriteMode ();
    AutoCorr.SetWriteMode ();

    while (Lower < PESpeech->SampleCount ()) {
        if (Upper > PESpeech->SampleCount ()) {   // adjust for end of data
            Upper = PESpeech->SampleCount ();
        }

        if ((Upper - Lower) < MILLISECOND(20)) break;

                                        // copy a frame
        for (Index = Lower; Index < Upper; ++Index) {
            FrameBuffer[Index - Lower] = (*PESpeech)[Index];
        }
                                        // generate LP coeffs for this frame
        Predict->FrameLPCoeff (FrameBuffer,
            (Upper - Lower), FrameLPC, FrameACR);

        for (Index = 0; Index < NCoeff; ++Index) {
            LPCoeff.AddSample (FrameLPC[Index]);    // add LPC to signal
            AutoCorr.AddSample (FrameACR[Index]);   // add R(0)..R(p-1)
        }

        AutoCorr.AddSample (FrameACR[Index]);       // ...and R(p)

        Lower += MILLISECOND(15);       // next frame (overlap of 5ms)
        Upper += MILLISECOND(15);
    }

    LPCoeff.SetReadMode ();             // set up to read
    AutoCorr.SetReadMode ();

    delete PESpeech;
    delete[] FrameBuffer;               // get rid of temp storage
    delete[] FrameLPC;
    delete[] FrameACR;
}

//**************************** LPANALYZER.CPP *******************************
