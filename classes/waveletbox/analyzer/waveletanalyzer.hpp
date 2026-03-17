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
// WAVELETANALYZER.HPP                                                      *
//                                                                          *
// Header for class `WaveletAnalyzer', which is an abstract base class      *
// interface for specific wavelet transforms.                               *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 8, 2001.                                       *
//                                                                          *
//***************************************************************************

#ifndef __WAVELETANALYZER_HPP
#define __WAVELETANALYZER_HPP

#include "../../signal/signal.hpp"

//***************************************************************************
class WaveletAnalyzer {
private:
    unsigned int NScale;                // highest scale to analyze signal

public:
    WaveletAnalyzer (unsigned int HScale);
    unsigned int HighestScale (void);   // returns highest scale

    virtual void AnalyzeDetail (Signal& Raw,
        Signal **Detail) = 0;
};

#endif

//************************** WAVELETANALYZER.HPP ****************************
