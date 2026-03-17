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
// PITCHDETECTOR.HPP                                                        *
//                                                                          *
// Header file for class PitchDetector, which implements the Rabiner-Gold   *
// pitch detection algorithm.                                               *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 19, 2002.                                      *
//                                                                          *
//***************************************************************************

#ifndef __PITCHDETECTOR_HPP
#define __PITCHDETECTOR_HPP

#include "../../signal/signal.hpp"
#include "../../filter/filter.hpp"
#include "../../filter/fir/firfilter.hpp"
#include "impulsegenerator/impulsegenerator.hpp"
#include "pitchextractor/pitchextractor.hpp"

#define     NPITCHEXTRACTORS    6
#define     NPITCHESTIMATES     6
#define     NRANGES             4
#define     NWINDOWS            4

//***************************************************************************
class PitchDetector {
private:
    Filter *LPF;                        // lowpass prefilter
    ImpulseGenerator *ImpGen;           // impulse train generator
    PitchExtractor *PitchExt[NPITCHEXTRACTORS]; // six pitch extractors
    Signal *Pitch;                      // holds final pitch

                                        // final pitch estimator
    float FindPitch (float *PEstimate[NPITCHEXTRACTORS]);

public:
    PitchDetector (Signal& Speech);     // pass speech signal
    ~PitchDetector ();

    Signal& FinalPitch (void);          // returns final pitch array
};

#endif

//************************** PITCHDETECTOR.HPP ******************************
