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
// USERAPP.CPP                                                              *
//                                                                          *
// User interaction module for the Speaker Identification Project.          *
// A front-end utility that provides users access to the system.            *
//                                                                          *
// Last modified on April 18, 2002.                                         *
//                                                                          *
//***************************************************************************

#include <conio.h>
#include "classes/uinterface/console/console.hpp"
#include "classes/uinterface/menugeneric/usermainmenu/usermainmenu.hpp"

int main (void) {
    MenuGeneric *User;

    clrscr ();
    textattr (REVERSE_VIDEO);
    clreol ();
    cputs (" Speaker Identification System (User Module)");
    textattr (NORMAL_VIDEO);

    User = new UserMainMenu (9, 29);
    User->Execute ();
    delete User;

    clrscr ();
    return (0);
}

//***************************** USERAPP.CPP *********************************
