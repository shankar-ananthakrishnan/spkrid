/****************************************************************************
 *                                                                          *
 * SPKRID, A Prototype Text-Dependent Speaker Identification System.        *
 * Copyright (C) 2002 A.Sankaranarayanan.                                   *
 *                                                                          *
 * This file is part of SPKRID.                                             *
 *                                                                          *
 * SPKRID is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License              *
 * as published by the Free Software Foundation; either version 2           *
 * of the License, or (at your option) any later version.                   *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,                   *
 * MA  02111-1307, USA.                                                     *
 *                                                                          *
 * My e-mail address is: total_nerd@hotmail.com.                            *
 *                                                                          *
 *                                                                          *
 * READKEY.C                                                                *
 *                                                                          *
 * Routines for reading data from the keyboard.                             *
 * Non-portable routines (not ANSI C compliant).                            *
 * User interface component for the Speaker Identification Project.         *
 *                                                                          *
 * Last modified on April 18, 2002.                                         *
 *                                                                          *
 ****************************************************************************/

#include <conio.h>
#include <string.h>

#include "keycode.h"

#define    TRUE    1
#define    FALSE   0

/****************************************************************************/
unsigned int ReadKey (void) {
    unsigned int Key;

    if ((Key = getch ()) == 0) {        /* extended key ? */
        Key |= getch () << 8;           /* yes, read next code */
    }

    return (Key);
}

/****************************************************************************/
unsigned int ReadString (char *Str, int MaxLen) {
    int Index;                          /* current location in string */
    int Valid;                          /* last valid index */
    unsigned int Y, X;
    unsigned int Key;                   /* key pressed */
    int Temp;
    char Clear[81];
    char Charact;                       /* character pressed */
    unsigned char Update;

    _setcursortype (_SOLIDCURSOR);

    Y = wherey ();                      /* where are we ? */
    X = wherex ();
    for (Index = 0; Index < MaxLen; ++Index) Clear[Index] = ' ';
    Clear[MaxLen] = '\0';
    Index = strlen (Str);
    Valid = Index - 1;
    Update = TRUE;

    do {
        if (Update) {                   /* time to update ? */
            gotoxy (X, Y);
            cputs (Clear);              /* clear till end of field */
            gotoxy (X, Y);
            cputs (Str);                /* partial text entered */
        }

        gotoxy (X + Index, Y);

        Key = ReadKey ();               /* read next keystroke */
        Update = FALSE;

        switch (Key) {                      /* check what was pressed */
            case KEY_LT_ARROW:
                if (Index > 0) --Index;     /* left, push cursor back */
                break;

            case KEY_RT_ARROW:
                if (Index <= Valid) ++Index;    /* right, push ahead */
                break;

            case KEY_HOME:
                Index = 0; break;           /* go to start */

            case KEY_END:
                Index = Valid + 1; break;   /* go to end */

            case KEY_BACKSPACE:         /* push back and delete prev char */
                if (Index > 0) {
                    for (Temp = --Index; Temp < Valid; ++Temp) {
                        Str[Temp] = Str[Temp + 1];
                    }
                    Str[Valid--] = '\0';
                    Update = TRUE;
                }

                break;

            case KEY_DELETE:            /* push back and delete this char */
                if (Index <= Valid) {
                    for (Temp = Index; Temp < Valid; ++Temp) {
                        Str[Temp] = Str[Temp + 1];
                    }
                    Str[Valid--] = '\0';
                    Update = TRUE;
                }

                break;

            default:                    /* check if valid char & insert */
                if (((Charact = Key & 0xFF) != 0) && (Index < MaxLen)) {
                    if (Charact >= ' ' && Charact <= '{') {
                        Str[Index++] = Charact;
                        if (Index > Valid) Valid = Index - 1;
                        Str[Valid + 1] = '\0';
                        Update = TRUE;
                    }
                }
        }                               /* loop till one of these is hit */
    } while (Key != KEY_ENTER && Key != KEY_ESCAPE &&
        Key != KEY_CTRL_TAB && Key != KEY_TAB);

    return (Key);                       /* return keystroke */
}

/******************************* READKEY.C **********************************/
