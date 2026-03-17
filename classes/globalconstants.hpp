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
// GLOBALCONSTANTS.HPP                                                      *
//                                                                          *
// Defines constants that are used by many modules in the system.           *
// Fundamental component of the Speaker Identification Project.             *
//                                                                          *
// Last modified on April 25, 2002.                                         *
//                                                                          *
//***************************************************************************

#ifndef __GLOBALCONSTANTS_HPP
#define __GLOBALCONSTANTS_HPP

// The threshold decides whether a person is to be successfully identified
// by the system if his/her profile bears the minimum distance to the test
// profile. This is only a test value, subject to change as the distance
// algorithms are refined.

#define     THRESHOLD            100.0
#define     ADMIN_THRESHOLD      100.0

// Audio related constants and macros

#define     AUDIO_SAMPLE_RATE   11025
#define     AUDIO_SAMPLE_SIZE   8
#define     MILLISECOND(x)      ((x) * AUDIO_SAMPLE_RATE / 1000)
#define     AUDIO_FRAME_SIZE    MILLISECOND(20)
#define     SAMPLE_INTERVAL     (1.0 / AUDIO_SAMPLE_RATE)

#define     NLPCOEFFICIENTS     12      // # of linear predictor coefficients
#define     NLEVELS              7      // # of wavelet analysis levels

// Miscellaneous constants

#define     MAX_PROFILE_LEN     50

// Relative system paths for various resources

#define     DATABASE_INDEX      "profiles.ind"
#define     DATABASE_PATH       "profiles\\"
#define     MENU_PATH           "uidata\\menus\\"
#define     DIALOG_PATH         "uidata\\dialogs\\"
#define     AUDIO_PATH          "uidata\\audio\\"

#endif

//************************* GLOBALCONSTANTS.HPP *****************************
