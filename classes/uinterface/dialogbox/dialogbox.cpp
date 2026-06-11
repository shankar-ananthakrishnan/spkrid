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
// DIALOGBOX.CPP                                                            *
//                                                                          *
// Implementation of class `DialogBox', which accepts input from the user.  *
// User interface component of the Speaker Identification Project.          *
//                                                                          *
// Last modified on March 12, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <stdio.h>
#include <string.h>
#include "../console/conio_compat.h"

#include "../console/console.hpp"
#include "../console/keycode.hpp"
#include "dialogbox.hpp"

//***************************************************************************
DialogBox::DialogBox (char *DialogName, unsigned int Y,
    unsigned int X) : Top(Y), Left(X) {
    FILE *DialogFile;                   // dialog box resource file
    unsigned int Width;                 // dimensions of dialog
    unsigned int Height;
    unsigned int RelY, RelX;            // relative X-Y of items
    unsigned int MaxLen;                // length of field
    unsigned int Index;
    char TempPrompt[51];                // prompt text
    char TempInit[51];                  // initial string value
    char Title[51];                     // dialog title

    DialogFileName = new char [strlen (DialogName) + 5];
    strcpy (DialogFileName, DialogName);
    strcat (DialogFileName, DIALOG_EXTENSION);

    DialogFile = fopen (DialogFileName, "rt");      // open resource
    fscanf (DialogFile, "%s%d%d%d", Title, &NPrompts, &Height, &Width);

    UserInput = new char * [NPrompts];  // create memory for initial strings

    for (Index = 0; Index < NPrompts; ++Index) {
        fscanf (DialogFile, "%d%d%s%d%s", &RelY, &RelX,
            TempPrompt, &MaxLen, TempInit);
        UserInput[Index] = new char [MaxLen + 1];

        *strchr (TempInit, '*') = '\0';         // scan & terminate at *
        strcpy (UserInput[Index], Expand (TempInit));
    }

    fclose (DialogFile);
}

//***************************************************************************
DialogBox::~DialogBox () {
    unsigned int Index;

    for (Index = 0; Index < NPrompts; ++Index) {
        delete[] UserInput[Index];      // get rid of text values
    }

    delete[] UserInput;
    delete[] DialogFileName;
}

//***************************************************************************
char **DialogBox::Engage (void) {
    FILE *DialogFile;
    unsigned int Width;
    unsigned int Height;
    unsigned int Index;
    char **PromptText;
    int *MaxLen;
    unsigned int *RelY;
    unsigned int *RelX;
    char TempPrompt[51];
    char TempInit[51];
    char Title[51];
    unsigned int Result;
    char *TextBuffer;

// Open the dialog resource; read info and create the dialog box.

    DialogFile = fopen (DialogFileName, "rt");
    fscanf (DialogFile, "%s%d%d%d", Title, &NPrompts, &Height, &Width);

    PromptText = new char * [NPrompts];
    MaxLen = new int [NPrompts];
    RelY = new unsigned int [NPrompts];
    RelX = new unsigned int [NPrompts];

    TextBuffer = new char [(Height + 1) * (Width + 1) * sizeof (chtype)];
    gettext (Left, Top, Left + Width, Top + Height, TextBuffer);
    DrawBox (Top, Left, Top + Height, Left + Width);
    textattr (REVERSE_VIDEO);
    gotoxy (Left + (Width - strlen (Title)) / 2, Top);
    cputs (Expand (Title));
    textattr (NORMAL_VIDEO);

    for (Index = 0; Index < NPrompts; ++Index) {
        fscanf (DialogFile, "%d%d%s%d%s", &RelY[Index],
            &RelX[Index], TempPrompt, &MaxLen[Index], TempInit);
        PromptText[Index] = new char [strlen (TempPrompt) + 1];
        strcpy (PromptText[Index], Expand (TempPrompt));
        gotoxy (Left + RelX[Index], Top + RelY[Index]);
        highvideo ();
        cputs (PromptText[Index]);
        normvideo ();
        cputs (UserInput[Index]);
    }

    fclose (DialogFile);

    Index = 0;                          // default first field

    do {
        gotoxy (Left + RelX[Index] +
            strlen (PromptText[Index]), Top + RelY[Index]);

        Result = ReadString (UserInput[Index], MaxLen[Index]);

        switch (Result) {           // how did user terminate `ReadString'
            case KEY_TAB:           // a Tab ? go to the next field
                if (Index < NPrompts - 1) ++Index;
                break;

            case KEY_CTRL_TAB:      // Ctrl-Tab ? go to previous field
                if (Index > 0) --Index;
                break;
        }                           // till Enter or Escape is pressed
    } while (Result != KEY_ENTER && Result != KEY_ESCAPE);

                                            // restore old text
    puttext (Left, Top, Left + Width, Top + Height, TextBuffer);
    delete[] TextBuffer;

    for (Index = 0; Index < NPrompts; ++Index) {
        delete[] PromptText[Index];
    }

    delete[] PromptText;
    delete[] MaxLen;
    delete[] RelY;
    delete[] RelX;

    if (Result == KEY_ESCAPE) return (NULL);

    return (UserInput);
}

//****************************** DIALOGBOX.CPP ******************************
