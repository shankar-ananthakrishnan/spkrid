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
// DIALOGBOX.HPP                                                            *
//                                                                          *
// Header file for class `DialogBox', which accepts input from the user.    *
// User interface component of the Speaker Identification Project.          *
//                                                                          *
// Last modified on March 9, 2002.                                          *
//                                                                          *
//***************************************************************************

// `DialogBox' can be used to create versatile dialog boxes that can accept
// only text input. Considerable control can be exerted over the nature of
// the dialog boxes in terms of the field prompts, their positions in the
// dialog, the number of characters/field, initial values for these, and
// so on. See the file `DIALOG.TXT' for information on dialog resource
// formats.

#ifndef __DIALOGBOX_HPP
#define __DIALOGBOX_HPP

#define    DIALOG_EXTENSION    ".dlg"

//***************************************************************************
class DialogBox {
private:
    char *DialogFileName;               // name of dialog resource file
    char **UserInput;                   // array of user entered strings
    unsigned int Top;                   // location of dialog
    unsigned int Left;
    unsigned int NPrompts;              // number of prompts

public:
    DialogBox (char *DialogName, unsigned int Top,
        unsigned int Left);
    ~DialogBox ();

    char **Engage (void);               // engages user in dialog
};

#endif

//**************************** DIALOGBOX.HPP ********************************
