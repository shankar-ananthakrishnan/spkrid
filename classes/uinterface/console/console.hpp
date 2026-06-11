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
// CONSOLE.HPP                                                              *
//                                                                          *
// Header file for prototypes of miscellaneous console functions.           *
// User interface component of the Speaker Identification Project.          *
//                                                                          *
// Last modified on March 8, 2002.                                          *
//                                                                          *
//***************************************************************************

#ifndef __CONSOLE_HPP
#define __CONSOLE_HPP

#include "conio_compat.h"

#define    REVERSE_VIDEO    ((LIGHTGRAY << 4) | BLACK)
#define    NORMAL_VIDEO     ((BLACK << 4) | LIGHTGRAY)

extern "C" {
    unsigned int ReadKey (void);
    unsigned int ReadString (char *Str, unsigned int MaxLen);
    void DrawBox (unsigned int Top, unsigned int Left,
        unsigned int Bottom, unsigned int Right);
    char *Expand (char *Text);
};

#endif

//***************************** CONSOLE.HPP *********************************
