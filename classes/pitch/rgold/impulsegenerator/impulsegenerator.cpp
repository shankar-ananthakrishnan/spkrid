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
// IMPULSEGENERATOR.CPP                                                     *
//                                                                          *
// Implementation of the ImpulseGenerator class, which generates six        *
// impulse trains for the parallel pitch estimators.                        *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 15, 2002.                                      *
//                                                                          *
//***************************************************************************

#include "impulsegenerator.hpp"

#define NTRAINS 6                       // number of impulse trains

//***************************************************************************
ImpulseGenerator::ImpulseGenerator (Signal& Speech) {
    unsigned int Index;

    Impulse = new Signal * [NTRAINS];    // allocate memory for the trains
    for (Index = 0; Index < NTRAINS; ++Index) {
        Impulse[Index] = new Signal;
    }
                                        // call helper to actually
    GenImpulseTrains (Speech);          // generate impulses
}

//***************************************************************************
ImpulseGenerator::~ImpulseGenerator () {
    unsigned int Index;

    for (Index = 0; Index < NTRAINS; ++Index) {
        delete Impulse[Index];          // remove impulse trains
    }

    delete[] Impulse;
}

//***************************************************************************
Signal **ImpulseGenerator::ImpulseTrains (void) {
    return (Impulse);                   // toss back pointer to trains
}

//***************************************************************************
void ImpulseGenerator::GenImpulseTrains (Signal& Speech) {
    unsigned int Index;
    unsigned int Train;
    float PrevPeak;
    float PrevValley;
    float Temp;
    float LastSample;
    float ThisSample;
    float NextSample;
    bool NoPeak;
    bool NoValley;

    Speech.SetReadMode ();

    for (Train = 0; Train < NTRAINS; ++Train) {
        Impulse[Train]->AddSample (0.0);
    }

    PrevPeak = PrevValley = 0.0;

    ThisSample = Speech[0];
    NextSample = Speech[1];

// Traverse the speech signal chunk, detecting local peaks and valleys.
// When a peak or a valley is detected, the impulse trains are suitably
// modified, and PrevPeak and PrevValley are updated.

    for (Index = 2; Index < Speech.SampleCount (); ++Index) {
        NoPeak = NoValley = true;

        LastSample = ThisSample;
        ThisSample = NextSample;
        NextSample = Speech[Index];

// Peak detected. Modify impulse trains #0,1,2 and update PrevPeak

        if ((ThisSample > LastSample) &&
            (ThisSample > NextSample) &&
            (ThisSample > 0.0)) {

            Impulse[0]->AddSample (ThisSample);
            Impulse[1]->AddSample (ThisSample - PrevValley);
            Temp = ThisSample - PrevPeak;
            if (Temp < 0.0) Temp = 0.0;
            Impulse[2]->AddSample (Temp);

            PrevPeak = ThisSample;
            NoPeak = false;
        }

// Valley detected. Modify impulse trains #3,4,5 and update PrevValley

        if ((ThisSample < LastSample) &&
            (ThisSample < NextSample) &&
            (ThisSample < 0.0)) {

            Impulse[3]->AddSample (-ThisSample);
            Impulse[4]->AddSample (-ThisSample + PrevPeak);
            Temp = -ThisSample + PrevValley;
            if (Temp < 0.0) Temp = 0.0;
            Impulse[5]->AddSample (Temp);

            PrevValley = ThisSample;
            NoValley = false;
        }

        if (NoPeak) {
            for (Train = 0; Train < 3; ++Train) {
                Impulse[Train]->AddSample (0.0);
            }
        }

        if (NoValley) {
            for (Train = 3; Train < 6; ++Train) {
                Impulse[Train]->AddSample (0.0);
            }
        }
    }

    for (Train = 0; Train < NTRAINS; ++Train) {
        Impulse[Train]->SetReadMode ();
    }
}

//************************* IMPULSEGENERATOR.CPP ****************************
