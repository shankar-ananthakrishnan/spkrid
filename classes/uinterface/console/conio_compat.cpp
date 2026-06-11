//***************************************************************************
//
// CONIO_COMPAT.CPP
//
// Implementation of gettext() and puttext() using ncurses mvinch/mvaddch.
//
//***************************************************************************

#include "conio_compat.h"
#include <string.h>

/*
 * gettext: save region (x1,y1)-(x2,y2) (1-based conio coords) into buf.
 * buf is treated as a chtype array laid out row by row.
 */
void gettext (int x1, int y1, int x2, int y2, char *buf) {
    chtype *cbuf = (chtype *)buf;
    int row, col;
    int idx = 0;

    for (row = y1 - 1; row <= y2 - 1; ++row) {
        for (col = x1 - 1; col <= x2 - 1; ++col) {
            cbuf[idx++] = mvinch (row, col);
        }
    }
}

/*
 * puttext: restore region (x1,y1)-(x2,y2) from buf.
 */
void puttext (int x1, int y1, int x2, int y2, char *buf) {
    chtype *cbuf = (chtype *)buf;
    int row, col;
    int idx = 0;

    for (row = y1 - 1; row <= y2 - 1; ++row) {
        for (col = x1 - 1; col <= x2 - 1; ++col) {
            mvaddch (row, col, cbuf[idx++]);
        }
    }

    refresh ();
}

//************************** CONIO_COMPAT.CPP *******************************
