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
// MENUGENERIC.HPP                                                          *
//                                                                          *
// Header file for class `MenuGeneric', an abstract base class (interface)  *
// for displaying menus and accepting choices.                              *
// User interface component of the Speaker Identification Project.          *
//                                                                          *
// Last modified on April 18, 2002.                                         *
//                                                                          *
//***************************************************************************

#ifndef __MENUGENERIC_HPP
#define __MENUGENERIC_HPP

//
// The `MenuGeneric' class handles menus described by `menu files'. These
// files have a `.MNU' extension and are to be organized as follows:
//
//                          <N> <L>
//                          Choice-1-String
//                          Choice-2-String
//                                ...  
//                          Choice-N-String
//
// where N = number of menu choices (integer)
//       L = uniform length of each choice (integer)
//
// and each is separated by whitespace. `Menu names' passed to the construc-
// tor have the same names as the corresponding menu files, but without the
// extension. Spaces are not allowed in the choice strings; a `+' character
// is interpreted as a space.
//

#define     MENU_EXTENSION      ".mnu"

//***************************************************************************
class MenuGeneric {
private:
    unsigned int Top;
    unsigned int Left;
    char **MenuOption;
    unsigned int MaxChoice;
    unsigned int ChoiceLen;
    char *TextBuffer;

public:
    MenuGeneric (char *MenuName, unsigned int Y, unsigned int X);
    ~MenuGeneric ();

    unsigned int Interact (void);       // returns user choice
    virtual void Execute (void) = 0;    // menu-specific (subclass)
};

#endif

//**************************** MENUGENERIC.HPP ******************************
