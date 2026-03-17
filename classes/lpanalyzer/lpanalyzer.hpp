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
// LPANALYZER.HPP                                                           *
//                                                                          *
// Header file for the LPAnalyzer class, which takes in a speech signal and *
// produces a signal containing linear predictor coeffs for speech frames.  *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 24, 2002.                                      *
//                                                                          *
//***************************************************************************

#ifndef __LPANALYZER_HPP
#define __LPANALYZER_HPP

#include "../signal/signal.hpp"
#include "lpredictor/linearpredictor.hpp"

//***************************************************************************
class LPAnalyzer {
private:
    unsigned int NCoeff;
    LinearPredictor *Predict;           // single frame predictor

public:
    LPAnalyzer (unsigned int CoeffPerFrame);    // coefficients per frame
    ~LPAnalyzer ();

    void PredictorCoeff (Signal& Speech,
        Signal& LPCoeff, Signal& AutoCorr);
};

#endif

//***************************** LPANALYZER.HPP ******************************
