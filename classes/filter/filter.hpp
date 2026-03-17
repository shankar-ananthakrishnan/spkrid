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
// FILTER.HPP                                                               *
//                                                                          *
// Header file for the `Filter' abstract base class, which provides an      *
// interface to IIR and FIR digital filters.                                *
// Fundamental component of the Speaker Identification project.             *
//                                                                          *
// Last modified on January 20, 2002.                                       *
//                                                                          *
//***************************************************************************

#ifndef __FILTER_HPP
#define __FILTER_HPP

#include "../signal/signal.hpp"

class Filter {
public:
    virtual void ApplyFilter (Signal& ISignal, Signal& OSignal) = 0;
};

#endif

//******************************* FILTER.HPP ********************************
