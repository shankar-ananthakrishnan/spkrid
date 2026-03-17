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
// IIRFILTER.HPP                                                            *
//                                                                          *
// Header file for the `IIR_Filter' class, derived from the `Filter' base   *
// class, that implements IIR digital filters.                              *
// Fundamental component of the Speaker Identification project.             *
//                                                                          *
// Last modified on January 21, 2002.                                       *
//                                                                          *
//***************************************************************************

// The IIR digital filters are implemented by the method `ApplyFilter'
// as cascaded second order sections in order to minimize the parameter
// quantization effects. The user has to precalculate the second order
// section coefficients and pass those as parameters in N x 2 arrays
// where N is the number of second order sections. In case the filter
// is of odd order (leaving a lone 1st order section), a zero must be
// padded to convert it into an even order section.

#ifndef __IIRFILTER_HPP
#define __IIRFILTER_HPP

#include "../../signal/signal.hpp"
#include "../filter.hpp"

class IIR_Filter : public Filter {
private:
    unsigned int Numer;                 // 2nd order sections in numerator
    unsigned int Denom;                 //   and in denominator

    float **A;                          // feedback coefficients
    float **B;                          // feedforward coefficients
    float StageGain;                    // gain per section

public:
    IIR_Filter (float FFCoeff[][2], unsigned int NOrder,
                float FBCoeff[][2], unsigned int DOrder,
                float Gain);

    ~IIR_Filter ();

    virtual void ApplyFilter (Signal& ISignal, Signal& OSignal);
};

#endif

//**************************** IIRFILTER.HPP ********************************
