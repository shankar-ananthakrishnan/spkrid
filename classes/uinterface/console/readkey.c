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
 *                                                                          *
 * READKEY.C                                                                *
 *                                                                          *
 * Routines for reading data from the keyboard.                             *
 * Rewritten for ncurses (Linux port).                                      *
 *                                                                          *
 * Last modified on April 18, 2002.                                         *
 *                                                                          *
 ****************************************************************************/

#include <string.h>
#include "conio_compat.h"
#include "keycode.h"

#define    TRUE    1
#define    FALSE   0

/****************************************************************************/
unsigned int ReadKey (void) {
    int ch = wgetch (stdscr);

    switch (ch) {
        case KEY_UP:               return KEY_UP_ARROW;
        case KEY_DOWN:             return KEY_DN_ARROW;
        case KEY_LEFT:             return KEY_LT_ARROW;
        case KEY_RIGHT:            return KEY_RT_ARROW;
        case NCURSES_KEY_HOME:     return KEY_HOME;
        case NCURSES_KEY_END:      return KEY_END;
        case KEY_DC:               return KEY_DELETE;
        case NCURSES_KEY_BACKSPACE:
        case 8:                    return KEY_BACKSPACE;
        case '\t':                 return KEY_TAB;
        case KEY_BTAB:             return KEY_CTRL_TAB;
        case '\r':
        case '\n':
        case NCURSES_KEY_ENTER:    return KEY_ENTER;
        case 27:                   return KEY_ESCAPE;
        default:                   return (unsigned int)ch;
    }
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
