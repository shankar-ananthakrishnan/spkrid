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
 * DRAWBOX.C                                                                *
 *                                                                          *
 * Routine to draw a closed box on the screen using extended ASCII charact- *
 * ers. Non-portable routine (not ANSI C compliant). Works on Win/DOS.      *
 * User interface component of the Speaker Identification Project.          *
 *                                                                          *
 * Last modified on March 8, 2002.                                          *
 *                                                                          *
 ****************************************************************************/

#include <conio.h>

void DrawBox (unsigned int Top, unsigned int Left,
    unsigned int Bottom, unsigned int Right) {
    unsigned int X, Y;
    unsigned int Width;
    char Horz[81];
    char Vert[81];

    Width = Right - Left - 1;
    Horz[Width + 2] = '\0';

    Horz[0] = 'Ú'; Horz[Width + 1] = '¿';
    for (X = 1; X <= Width; ++X) Horz[X] = 'Ä';

    Vert[0] = '³'; Vert[Width + 1] = '³';
    for (X = 1; X <= Width; ++X) Vert[X] = ' ';
    Vert[Width + 2] = '\0';

    gotoxy (Left, Top);
    cputs (Horz);

    for (Y = Top + 1; Y < Bottom; ++Y) {
        gotoxy (Left, Y);
        cputs (Vert);
    }

    Horz[0] = 'À'; Horz[Width + 1] = 'Ù';
    gotoxy (Left, Bottom);
    cputs (Horz);

    return;
}

/******************************* DRAWBOX.C **********************************/
