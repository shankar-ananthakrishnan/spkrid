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
// MENUGENERIC.CPP                                                          *
//                                                                          *
// Implementation of `MenuGeneric', an abstract base class (interface) for  *
// displaying menus and accepting choices.                                  *
// User interface component of the Speaker Identification Project.          *
//                                                                          *
// Last modified on April 18, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <stdio.h>
#include <string.h>
#include "../console/conio_compat.h"

#include "menugeneric.hpp"
#include "../console/console.hpp"
#include "../console/keycode.hpp"

//***************************************************************************
MenuGeneric::MenuGeneric (char *MenuName, unsigned int Y,
    unsigned int X) : Top(Y), Left(X) {
    char *MenuFileName;
    unsigned int Index;
    FILE *MenuFile;

    MenuFileName = new char [strlen (MenuName) + 5];
    strcpy (MenuFileName, MenuName);            // copy menu name
    strcat (MenuFileName, MENU_EXTENSION);      // append file extension

    MenuFile = fopen (MenuFileName, "rt");
    fscanf (MenuFile, "%d%d", &MaxChoice, &ChoiceLen);    // read params

    MenuOption = new char * [MaxChoice];
    for (Index = 0; Index < MaxChoice; ++Index) {
        MenuOption[Index] = new char [ChoiceLen];
        fscanf (MenuFile, "%s", MenuOption[Index]);
        Expand (MenuOption[Index]);
    }

    fclose (MenuFile);

    TextBuffer = new char [(MaxChoice + 2) * (ChoiceLen + 2) * sizeof (chtype)];
    gettext (Left, Top, Left + ChoiceLen + 1,
        Top + MaxChoice + 1, TextBuffer);
}

//***************************************************************************
MenuGeneric::~MenuGeneric () {
    unsigned int Index;

    puttext (Left, Top, Left + ChoiceLen + 1,
        Top + MaxChoice + 1, TextBuffer);

    for (Index = 0; Index < MaxChoice; ++Index) {
        delete[] MenuOption[Index];
    }

    delete[] MenuOption;
    delete[] TextBuffer;
}

//***************************************************************************
unsigned int MenuGeneric::Interact (void) {
    unsigned int Choice;
    unsigned int Index;
    unsigned int Key;

    _setcursortype (_NOCURSOR);
    DrawBox (Top, Left, Top + MaxChoice + 1, Left + ChoiceLen + 1);

    for (Index = 0; Index < MaxChoice; ++Index) {    // print choices
        gotoxy (Left + 1, Top + Index + 1);
        cputs (MenuOption[Index]);
    }

    Choice = 0;

    do {
        textattr (REVERSE_VIDEO);
        gotoxy (Left + 1, Top + Choice + 1);
        cputs (MenuOption[Choice]);     // highlight current choice

        Key = ReadKey ();               // read keystroke

        textattr (NORMAL_VIDEO);
        gotoxy (Left + 1, Top + Choice + 1);
        cputs (MenuOption[Choice]);     // remove highlight

        switch (Key) {                  // what key was pressed ?
            case KEY_UP_ARROW:
                if (Choice-- == 0) Choice = MaxChoice - 1;
                break;

            case KEY_DN_ARROW:
                if (Choice++ == MaxChoice - 1) Choice = 0;
                break;
        }

    } while (Key != KEY_ENTER && Key != KEY_ESCAPE);    // stop polling

    _setcursortype (_NORMALCURSOR);

    return (Key == KEY_ESCAPE) ? 0:(Choice + 1);
}

//***************************** MENUGENERIC.CPP *****************************
