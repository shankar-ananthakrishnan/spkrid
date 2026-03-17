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
// SILENCEREMOVER.HPP                                                       *
//                                                                          *
// Header for class `SilenceRemover', which clips the audio data input to   *
// remove silence and keeps only the voiced components of speech.           *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on April 25, 2002.                                         *
//                                                                          *
//***************************************************************************

#ifndef __SILENCEREMOVER_HPP
#define __SILENCEREMOVER_HPP

#include "../signal/signal.hpp"

#define SILENCE_THRESHOLD 3.0           // min. speech/silence energy to
                                        //   classify as speech
//***************************************************************************
class SilenceRemover {
private:
    float SNEnergy;                     // normalized energy of silence

public:
    SilenceRemover (Signal& SSample);   // give it a silence sample

    void ClipSilence (Signal& Raw,      // do the dirty work
        Signal& Clipped);
};

#endif

//************************** SILENCEREMOVER.HPP *****************************
