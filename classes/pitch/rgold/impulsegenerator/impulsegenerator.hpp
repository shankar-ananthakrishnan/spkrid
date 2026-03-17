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
// IMPULSEGENERATOR.HPP                                                     *
//                                                                          *
// Header file for the ImpulseGenerator class, which generates six impulse  *
// trains for the parallel pitch estimators.                                *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 15, 2002.                                      *
//                                                                          *
//***************************************************************************

#ifndef __IMPULSEGENERATOR_HPP
#define __IMPULSEGENERATOR_HPP

#include "../../../signal/signal.hpp"

//***************************************************************************
class ImpulseGenerator {
private:
    Signal **Impulse;                   // six impulse trains

    void GenImpulseTrains (Signal& Speech);    // helper to generate trains

public:
    ImpulseGenerator (Signal& Speech);
    ~ImpulseGenerator ();

    Signal **ImpulseTrains (void);      // returns impulse trains
};

#endif

//************************** IMPULSEGENERATOR.HPP ***************************
