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
// INFOBOX.CPP                                                              *
//                                                                          *
// Implementation of `InfoBox', used for displaying messages to the user.   *
// User interface component of the Speaker Identification Project.          *
//                                                                          *
// Last modified on April 18, 2002.                                         *
//                                                                          *
//***************************************************************************

#include "../console/conio_compat.h"
#include <string.h>

#include "../console/console.hpp"
#include "infobox.hpp"

//***************************************************************************
InfoBox::InfoBox (char *Title, unsigned int Y1, unsigned int X1,
    unsigned int Y2, unsigned int X2) : Top(Y1), Left(X1), Bottom(Y2),
    Right(X2), Y(1), X(1) {
    TextBuffer = new char [(Bottom - Top + 1) * (Right - Left + 1) * sizeof (chtype)];
    gettext (Left, Top, Right, Bottom, TextBuffer);     // save old text

    DrawBox (Top, Left, Bottom, Right);     // draw the box
    gotoxy (Left + (Right - Left - strlen (Title)) / 2, Top);
    textattr (REVERSE_VIDEO);
    cputs (Title);                          // print title
    textattr (NORMAL_VIDEO);
    gotoxy (Left + X, Top + Y);             // top left corner
}

//***************************************************************************
InfoBox::~InfoBox () {
    puttext (Left, Top, Right, Bottom, TextBuffer);     // restore text
    delete[] TextBuffer;
}

//***************************************************************************
void InfoBox::WipeClean (void) {
    unsigned int Index;
    char Space[81];

    for (Index = 0; Index < (Right - Left - 1); ++Index) {
        Space[Index] = ' ';             // construct clear string
    }

    Space[Index] = '\0';

    for (Index = Top + 1; Index < Bottom; ++Index) {
        gotoxy (Left + 1, Index);       // print clear string on every
        cputs (Space);                  //   row to clear the box
    }

    X = Y = 1;
    gotoxy (Left + 1, Top + 1);         // reposition at top left
}

//***************************************************************************
void InfoBox::MoveCursor (unsigned int RelY, unsigned int RelX) {
    X = RelX; Y = RelY;
    gotoxy (Left + X, Top + Y);         // set cursor relative to top left
}

//***************************************************************************
void InfoBox::WriteText (char *Text) {
    unsigned int Index;

    _setcursortype (_NOCURSOR);

    for (Index = 0; Text[Index] != '\0'; ++Index) {
        putch (Text[Index]);            // write next character
        if (++X == (Right - Left)) {    // next X, check for overshoot
            X = 1;                      // wrap to left
            if (++Y == (Bottom - Top)) {    // check for Y overshoot
                Y = 1;                  // wrap to top
                WipeClean ();           // clear window
            }
            gotoxy (Left + X, Top + Y);      // reposition cursor
        }
    }
}

//***************************************************************************
void InfoBox::AlignCentre (char *Text) {
    MoveCursor (Y, (Right - Left - strlen (Text)) / 2);
    WriteText (Text);
}

//****************************** INFOBOX.CPP ********************************
