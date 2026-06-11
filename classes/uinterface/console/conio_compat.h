//***************************************************************************
//
// CONIO_COMPAT.H
//
// ncurses-based compatibility layer replacing Borland conio.h.
// Provides clrscr, gotoxy, cputs, textattr, gettext/puttext, etc.
//
//***************************************************************************

#ifndef __CONIO_COMPAT_H
#define __CONIO_COMPAT_H

#include <ncurses.h>

/* conio colour constants */
#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define BROWN        6
#define LIGHTGRAY    7
#define DARKGRAY     8
#define LIGHTBLUE    9
#define LIGHTGREEN  10
#define LIGHTCYAN   11
#define LIGHTRED    12
#define LIGHTMAGENTA 13
#define YELLOW      14
#define WHITE       15

/* Borland cursor type constants */
#define _NOCURSOR     0
#define _NORMALCURSOR 1
#define _SOLIDCURSOR  2

/* conio function wrappers */
static inline void clrscr (void)           { clear (); refresh (); }
static inline void clreol (void)           { clrtoeol (); refresh (); }
static inline void gotoxy (int x, int y)   { move (y - 1, x - 1); }
static inline int  wherex (void)           { return getcurx (stdscr) + 1; }
static inline int  wherey (void)           { return getcury (stdscr) + 1; }
static inline void cputs (const char *s)   { addstr (s); refresh (); }
static inline void putch (int c)           { addch ((chtype)c); refresh (); }
static inline void highvideo (void)        { attron (A_BOLD); }
static inline void normvideo (void)        { attroff (A_BOLD); }
static inline void _setcursortype (int t)  { curs_set (t); }

/*
 * textattr: the codebase only uses REVERSE_VIDEO and NORMAL_VIDEO.
 * REVERSE_VIDEO = ((LIGHTGRAY << 4) | BLACK) = 0x70 = 112
 * NORMAL_VIDEO  = ((BLACK << 4) | LIGHTGRAY) = 0x07 = 7
 */
static inline void textattr (int attr) {
    if (attr == ((LIGHTGRAY << 4) | BLACK))
        attron (A_REVERSE);
    else
        attroff (A_REVERSE);
    refresh ();
}

/*
 * Freeze ncurses KEY_* values as enum constants before keycode.h overwrites
 * the macro names KEY_ENTER, KEY_BACKSPACE, KEY_HOME, and KEY_END.
 * Enums capture the value at definition time, unlike #define aliases.
 */
enum {
    NCURSES_KEY_BACKSPACE = KEY_BACKSPACE,
    NCURSES_KEY_HOME      = KEY_HOME,
    NCURSES_KEY_END       = KEY_END,
    NCURSES_KEY_ENTER     = KEY_ENTER
};

/*
 * gettext / puttext: save and restore a rectangular region of the screen.
 * The buffer stores chtype values (one per cell).
 * Buffer must be at least (rows * cols * sizeof(chtype)) bytes.
 * Coordinates are 1-based (conio convention).
 */
#ifdef __cplusplus
extern "C" {
#endif

void gettext (int x1, int y1, int x2, int y2, char *buf);
void puttext (int x1, int y1, int x2, int y2, char *buf);

#ifdef __cplusplus
}
#endif

#endif /* __CONIO_COMPAT_H */

//************************** CONIO_COMPAT.H *********************************
