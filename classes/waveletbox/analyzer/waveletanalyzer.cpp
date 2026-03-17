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
// WAVELETANALYZER.CPP                                                      *
//                                                                          *
// Implementation of `WaveletAnalyzer', an abstract base class interface    *
// for specific transforms using specific wavelets.                         *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 8, 2001.                                       *
//                                                                          *
//***************************************************************************

#include "waveletanalyzer.hpp"

//***************************************************************************
WaveletAnalyzer::WaveletAnalyzer
    (unsigned int HScale) : NScale(HScale) {}

//***************************************************************************
unsigned int WaveletAnalyzer::HighestScale (void) {
    return (NScale);
}

//************************** WAVELETANALYZER.CPP ****************************
