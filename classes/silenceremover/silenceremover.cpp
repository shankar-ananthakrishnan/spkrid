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
// SILENCEREMOVER.CPP                                                       *
//                                                                          *
// Implementation of class `SilenceRemover', which clips the audio data     *
// to remove silence and keep only the voiced components of speech.         *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on April 2, 2002.                                          *
//                                                                          *
//***************************************************************************

#include "../globalconstants.hpp"
#include "../signal/signal.hpp"
#include "silenceremover.hpp"

//***************************************************************************
SilenceRemover::SilenceRemover (Signal& SSample) {
    SNEnergy = SSample.Energy (0, SSample.SampleCount () - 1) /
            SSample.SampleCount ();
}

//***************************************************************************
void SilenceRemover::ClipSilence (Signal& Raw, Signal& Clipped) {
    float NFrameEnergy;                 // normalized frame energy
    unsigned int Index;
    unsigned int Lower;
    unsigned int Upper;

    Raw.SetReadMode ();                 // set signal modes
    Clipped.SetWriteMode ();

    Lower = MILLISECOND(0);             // initial frame
    Upper = MILLISECOND(20);

    while (Lower < Raw.SampleCount ()) {
        if (Upper >= Raw.SampleCount ()) {   // adjust for end of data
            Upper = Raw.SampleCount () - 1;
        }

        if ((Upper - Lower) < MILLISECOND(20)) break;

                                        // normalized energy for curr. frame
        NFrameEnergy = Raw.Energy (Lower, Upper) / (Upper - Lower);

                                        // classify as speech if > threshold
        if ((NFrameEnergy / SNEnergy) > SILENCE_THRESHOLD) {
            for (Index = Lower; Index < Upper; ++Index) {
                Clipped.AddSample (Raw[Index]);
            }
        }

        Lower += MILLISECOND(20);       // advance to next frame
        Upper += MILLISECOND(20);
    }

    Clipped.SetReadMode ();             // so user can read
}

//************************** SILENCEREMOVER.CPP *****************************
