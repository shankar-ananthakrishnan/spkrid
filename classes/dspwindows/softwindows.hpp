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
// SOFTWINDOWS.HPP                                                          *
//                                                                          *
// Prototypes for definitions of many types of "soft" DSP windows.          *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on February 8, 2001.                                       *
//                                                                          *
//***************************************************************************

#ifndef __SOFTWINDOWS_HPP
#define __SOFTWINDOWS_HPP

#include <math.h>

#ifndef PI
#define PI 3.14159
#endif

//***************************************************************************
inline float Hamming (unsigned int Index, unsigned int Length) {
    return (0.54 - 0.46 * cos (2 * PI * Index / (Length - 1)));
}

//***************************************************************************
inline float Hanning (unsigned int Index, unsigned int Length) {
    return (0.50 - 0.50 * cos (2 * PI * Index / (Length - 1)));
}

//***************************************************************************
inline float Blackman (unsigned int Index, unsigned int Length) {
    return (0.42 - 0.50 * cos (2 * PI * Index / (Length - 1)) +
                0.08 * cos (4 * PI * Index / (Length - 1)));
}

//***************************************************************************
inline float Bartlett (unsigned int Index, unsigned int Length) {
    return ((Index <= (Length - 1) / 2) ?
                (2 * Index / (Length - 1)):(2 - 2 * Index / (Length - 1)));
}

#endif

//*************************** SOFTWINDOWS.HPP *******************************
