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
// FIRFILTER.HPP                                                            *
//                                                                          *
// Header file for the `FIR_Filter' class, derived from the `Filter' base   *
// class, that implements FIR digital filters.                              *
// Fundamental component of the Speaker Identification project.             *
//                                                                          *
// Last modified on January 26, 2002.                                       *
//                                                                          *
//***************************************************************************

#ifndef __FIRFILTER_HPP
#define __FIRFILTER_HPP

#include "../../signal/signal.hpp"
#include "../filter.hpp"

//***************************************************************************
class FIR_Filter : public Filter {
private:
    unsigned int Order;                 // order of filter
    float *B;                           // feedforward coefficients

public:
    FIR_Filter (float FFCoeff[], unsigned int NOrder);
    ~FIR_Filter ();

    virtual void ApplyFilter (Signal& ISignal, Signal& OSignal);
};

#endif

//**************************** FIRFILTER.HPP ********************************
