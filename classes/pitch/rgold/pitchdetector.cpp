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
// PITCHDETECTOR.CPP                                                        *
//                                                                          *
// Implementation of class PitchDetector, for the Rabiner-Gold pitch        *
// detection algorithm.                                                     *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 21, 2002.                                      *
//                                                                          *
//***************************************************************************

#include <math.h>

#include "../../globalconstants.hpp"
#include "pitchdetector.hpp"

//***************************************************************************
PitchDetector::PitchDetector (Signal& Speech) {
    Signal *Filtered;                   // temporary filtered signal
    Signal **Trains;                    // pointers to impulse trains
    float *Estimate[NPITCHESTIMATES];   // pitch estimate matrix
    bool NotDone;
    unsigned int Index;

    static float LPFCoeff[] = {         // load lowpass filter coeffs
    #include "lpf600.coeff"
    };

    Filtered = new Signal;
    Pitch = new Signal;

    LPF = new FIR_Filter (LPFCoeff, 163);   // create filter
    LPF->ApplyFilter (Speech, *Filtered);   // process speech signal

    ImpGen = new ImpulseGenerator (*Filtered);  // generate impulse trains
    Trains = ImpGen->ImpulseTrains ();
                                            // create pitch extractors
    for (Index = 0; Index < NPITCHEXTRACTORS; ++Index) {
        PitchExt[Index] = new PitchExtractor (*Trains[Index]);
    }

    NotDone = true;

    do {                                // repetitively extract pitch
        for (Index = 0; (Index < NPITCHEXTRACTORS) && (NotDone); ++Index) {
            Estimate[Index] = PitchExt[Index]->PitchEstimateArray ();
            NotDone = (Estimate[Index] == NULL) ? false:true;
        }

        if (NotDone) {
            Pitch->AddSample (FindPitch (Estimate));
        }
    } while (NotDone);

    Pitch->SetReadMode ();              // set it up so user can read

    delete Filtered;                    // get rid of components used
    delete LPF;
    delete ImpGen;

    for (Index = 0; Index < NPITCHEXTRACTORS; ++Index) {
        delete PitchExt[Index];
    }
}

//***************************************************************************
PitchDetector::~PitchDetector () {
    delete Pitch;                       // get rid of pitch array
}

//***************************************************************************
Signal& PitchDetector::FinalPitch (void) {
    return (*Pitch);                    // toss back reference to pitch
}

//***************************************************************************
float PitchDetector::FindPitch (float *PEstimate[NPITCHEXTRACTORS]) {
    unsigned int PPEIndex;              // current pitch extractor
    unsigned int Row;
    unsigned int Col;
    unsigned int WIndex;                // window width selector
    unsigned int WinID;                 // current window
    int Match;                          // coincidences for this pitch/window
    int MaxPPEMatch;                    // highest coincidence for this pitch
    int BestMatch;                      // highest overall coincidence
    unsigned int BestMatchIndex;
                                        // window widths
    static float WindowWidth[NRANGES][NWINDOWS] = {
        {0.0001, 0.0002, 0.0003, 0.0004},       // 1.6  -  3.1 ms
        {0.0002, 0.0004, 0.0006, 0.0008},       // 3.1  -  6.3 ms
        {0.0004, 0.0008, 0.0012, 0.0016},       // 6.3  - 12.7 ms
        {0.0008, 0.0016, 0.0024, 0.0032}        // 12.7 - 25.5 ms
    };
                                        // pitch ranges
    static float PitchRange[NRANGES] = {
        0.0016,                         // select window 0
        0.0031,                         // select window 1
        0.0063,                         // select window 2
        0.0127                          // select window 3
    };
    
    static int Bias[NWINDOWS] = {1, 2, 5, 7};     // bias to compensate

    BestMatch = BestMatchIndex = 0;     // initial "best-match"

    for (PPEIndex = 0; PPEIndex < NPITCHEXTRACTORS; ++PPEIndex) {
                                        // select appropriate window
        for (WIndex = NWINDOWS - 1; (PEstimate[PPEIndex][0] <
                PitchRange[WIndex]) && (WIndex != 0); --WIndex);

        MaxPPEMatch = 0;                // highest coincidence for this PPE

        for (WinID = 0; WinID < NWINDOWS; ++WinID) {
            Match = 0;                  // coincidences for each window

            for (Row = 0; Row < NPITCHEXTRACTORS; ++Row) {
                for (Col = 0; Col < NPITCHESTIMATES; ++Col) {
                    if (fabs (PEstimate[PPEIndex][0] -
                        PEstimate[Row][Col]) <
                            (WindowWidth[WIndex][WinID] / 2.0)) {
                        ++Match;        // coincidence found
                    }
                }
            }

            Match -= (Bias[WinID] + 1);     // apply compensation bias
            if (MaxPPEMatch < Match) MaxPPEMatch = Match;
        }

        if (BestMatch < MaxPPEMatch) {      // highest match yet?
            BestMatch = MaxPPEMatch;        // no, update
            BestMatchIndex = PPEIndex;
        }
    }

    return (PEstimate[BestMatchIndex][0]);  // toss back best pitch estimate
}

//**************************** PITCHDETECTOR.CPP ****************************
