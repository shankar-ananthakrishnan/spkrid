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
 * DRAWBOX.C                                                                *
 *                                                                          *
 * Routine to draw a closed box on the screen using ncurses ACS characters. *
 * Linux port of the original CP437-based implementation.                   *
 *                                                                          *
 * Last modified on March 8, 2002.                                          *
 *                                                                          *
 ****************************************************************************/

#include "conio_compat.h"

void DrawBox (unsigned int Top, unsigned int Left,
    unsigned int Bottom, unsigned int Right) {
    unsigned int X, Y;

    /* top edge */
    mvaddch (Top - 1, Left - 1, ACS_ULCORNER);
    for (X = Left; X < Right; ++X)
        mvaddch (Top - 1, X, ACS_HLINE);
    mvaddch (Top - 1, Right - 1, ACS_URCORNER);

    /* side edges and interior blank lines */
    for (Y = Top; Y < Bottom; ++Y) {
        mvaddch (Y, Left - 1, ACS_VLINE);
        for (X = Left; X < Right; ++X)
            mvaddch (Y, X, ' ');
        mvaddch (Y, Right - 1, ACS_VLINE);
    }

    /* bottom edge */
    mvaddch (Bottom - 1, Left - 1, ACS_LLCORNER);
    for (X = Left; X < Right; ++X)
        mvaddch (Bottom - 1, X, ACS_HLINE);
    mvaddch (Bottom - 1, Right - 1, ACS_LRCORNER);

    refresh ();
}

/******************************* DRAWBOX.C **********************************/
