/*
 *   This is a curses module for Python.
 *
 *   Based on prior work by Lance Ellinghaus and Oliver Andrich
 *   Version 1.2 of this module: Copyright 1994 by Lance Ellinghouse,
 *    Cathedral City, California Republic, United States of America.
 *
 *   Version 1.5b1, heavily extended for ncurses by Oliver Andrich:
 *   Copyright 1996,1997 by Oliver Andrich, Koblenz, Germany.
 *
 *   Tidied for Python 1.6, and currently maintained by
 *   <akuchlin@mems-exchange.org>.
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this source file to use, copy, modify, merge, or publish it
 *   subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included
 *   in all copies or in any new file that contains a substantial portion of
 *   this file.
 *
 *   THE  AUTHOR  MAKES  NO  REPRESENTATIONS ABOUT  THE  SUITABILITY  OF
 *   THE  SOFTWARE FOR  ANY  PURPOSE.  IT IS  PROVIDED  "AS IS"  WITHOUT
 *   EXPRESS OR  IMPLIED WARRANTY.  THE AUTHOR DISCLAIMS  ALL WARRANTIES
 *   WITH  REGARD TO  THIS  SOFTWARE, INCLUDING  ALL IMPLIED  WARRANTIES
 *   OF   MERCHANTABILITY,  FITNESS   FOR  A   PARTICULAR  PURPOSE   AND
 *   NON-INFRINGEMENT  OF THIRD  PARTY  RIGHTS. IN  NO  EVENT SHALL  THE
 *   AUTHOR  BE LIABLE  TO  YOU  OR ANY  OTHER  PARTY  FOR ANY  SPECIAL,
 *   INDIRECT,  OR  CONSEQUENTIAL  DAMAGES  OR  ANY  DAMAGES  WHATSOEVER
 *   WHETHER IN AN  ACTION OF CONTRACT, NEGLIGENCE,  STRICT LIABILITY OR
 *   ANY OTHER  ACTION ARISING OUT OF  OR IN CONNECTION WITH  THE USE OR
 *   PERFORMANCE OF THIS SOFTWARE.
 */

/* CVS: $Id: _cursesmodule.c,v 1.1.1.1 2002/02/05 23:24:24 zarzycki Exp $ */

/*

A number of SysV or ncurses functions don't have wrappers yet; if you need
a given function, add it and send a patch.  Here's a list of currently
unsupported functions:

	addchnstr addchstr chgat color_set define_key
	del_curterm delscreen dupwin inchnstr inchstr innstr keyok
	mcprint mvaddchnstr mvaddchstr mvchgat mvcur mvinchnstr
	mvinchstr mvinnstr mmvwaddchnstr mvwaddchstr mvwchgat
	mvwgetnstr mvwinchnstr mvwinchstr mvwinnstr newterm
	resizeterm restartterm ripoffline scr_dump
	scr_init scr_restore scr_set scrl set_curterm set_term setterm
	tgetent tgetflag tgetnum tgetstr tgoto timeout tputs
	use_default_colors vidattr vidputs waddchnstr waddchstr wchgat
	wcolor_set winchnstr winchstr winnstr wmouse_trafo wscrl

Low-priority: 
	slk_attr slk_attr_off slk_attr_on slk_attr_set slk_attroff
	slk_attron slk_attrset slk_clear slk_color slk_init slk_label
	slk_noutrefresh slk_refresh slk_restore slk_set slk_touch

Menu extension (ncurses and probably SYSV):
	current_item free_item free_menu item_count item_description
	item_index item_init item_name item_opts item_opts_off
	item_opts_on item_term item_userptr item_value item_visible
	menu_back menu_driver menu_fore menu_format menu_grey
	menu_init menu_items menu_mark menu_opts menu_opts_off
	menu_opts_on menu_pad menu_pattern menu_request_by_name
	menu_request_name menu_spacing menu_sub menu_term menu_userptr
	menu_win new_item new_menu pos_menu_cursor post_menu
	scale_menu set_current_item set_item_init set_item_opts
	set_item_term set_item_userptr set_item_value set_menu_back
	set_menu_fore set_menu_format set_menu_grey set_menu_init
	set_menu_items set_menu_mark set_menu_opts set_menu_pad
	set_menu_pattern set_menu_spacing set_menu_sub set_menu_term
	set_menu_userptr set_menu_win set_top_row top_row unpost_menu

Form extension (ncurses and probably SYSV):
	current_field data_ahead data_behind dup_field
	dynamic_fieldinfo field_arg field_back field_buffer
	field_count field_fore field_index field_info field_init
	field_just field_opts field_opts_off field_opts_on field_pad
	field_status field_term field_type field_userptr form_driver
	form_fields form_init form_opts form_opts_off form_opts_on
	form_page form_request_by_name form_request_name form_sub
	form_term form_userptr form_win free_field free_form
	link_field link_fieldtype move_field new_field new_form
	new_page pos_form_cursor post_form scale_form
	set_current_field set_field_back set_field_buffer
	set_field_fore set_field_init set_field_just set_field_opts
	set_field_pad set_field_status set_field_term set_field_type
	set_field_userptr set_fieldtype_arg set_fieldtype_choice
	set_form_fields set_form_init set_form_opts set_form_page
	set_form_sub set_form_term set_form_userptr set_form_win
	set_max_field set_new_page unpost_form


 */

/* Release Number */

char *PyCursesVersion = "2.2";

/* Includes */

#include "Python.h"

#ifdef __osf__
#define _XOPEN_SOURCE_EXTENDED  /* Define macro for OSF/1 */
#define STRICT_SYSV_CURSES      /* Don't use ncurses extensions */
#endif

#ifdef __hpux
#define _XOPEN_SOURCE_EXTENDED
#define STRICT_SYSV_CURSES
#endif

#define CURSES_MODULE
#include "py_curses.h"

/*  These prototypes are in <term.h>, but including this header 
    #defines many common symbols (such as "lines") which breaks the 
    curses module in other ways.  So the code will just specify 
    explicit prototypes here. */
extern int setupterm(char *,int,int *);
#ifdef __sgi
#include <term.h>
#endif

#if !defined(HAVE_NCURSES_H) && (defined(sgi) || defined(__sun))
#define STRICT_SYSV_CURSES       /* Don't use ncurses extensions */
typedef chtype attr_t;           /* No attr_t type is available */
#endif

#if defined(_AIX)
#define STRICT_SYSV_CURSES
#endif

/* Definition of exception curses.error */

static PyObject *PyCursesError;

/* Tells whether setupterm() has been called to initialise terminfo.  */
static int initialised_setupterm = FALSE;

/* Tells whether initscr() has been called to initialise curses.  */
static int initialised = FALSE;

/* Tells whether start_color() has been called to initialise color usage. */
static int initialisedcolors = FALSE;

/* Utility Macros */
#define PyCursesSetupTermCalled \
  if (initialised_setupterm != TRUE) { \
                  PyErr_SetString(PyCursesError, \
                                  "must call (at least) setupterm() first"); \
                  return 0; }

#define PyCursesInitialised \
  if (initialised != TRUE) { \
                  PyErr_SetString(PyCursesError, \
                                  "must call initscr() first"); \
                  return 0; }

#define PyCursesInitialisedColor \
  if (initialisedcolors != TRUE) { \
                  PyErr_SetString(PyCursesError, \
                                  "must call start_color() first"); \
                  return 0; }

/* Utility Functions */

/*
 * Check the return code from a curses function and return None 
 * or raise an exception as appropriate.  These are exported using the
 * CObject API. 
 */

static PyObject *
PyCursesCheckERR(int code, char *fname)
{
  if (code != ERR) {
    Py_INCREF(Py_None);
    return Py_None;
  } else {
    if (fname == NULL) {
      PyErr_SetString(PyCursesError, catchall_ERR);
    } else {
      PyErr_Format(PyCursesError, "%s() returned ERR", fname);
    }
    return NULL;
  }
}

static int 
PyCurses_ConvertToChtype(PyObject *obj, chtype *ch)
{
  if (PyInt_Check(obj)) {
    *ch = (chtype) PyInt_AsLong(obj);
  } else if(PyString_Check(obj) 
	    && (PyString_Size(obj) == 1)) {
    *ch = (chtype) *PyString_AsString(obj);
  } else {
    return 0;
  }
  return 1;
}

/* Function versions of the 3 functions for tested whether curses has been
   initialised or not. */
   
static int func_PyCursesSetupTermCalled(void)
{
    PyCursesSetupTermCalled;
    return 1;
}

static int func_PyCursesInitialised(void)
{
    PyCursesInitialised;
    return 1;
}

static int func_PyCursesInitialisedColor(void)
{
    PyCursesInitialisedColor;
    return 1;
}

/*****************************************************************************
 The Window Object
******************************************************************************/

/* Definition of the window type */

PyTypeObject PyCursesWindow_Type;

/* Function prototype macros for Window object

   X - function name
   TYPE - parameter Type
   ERGSTR - format string for construction of the return value
   PARSESTR - format string for argument parsing
   */

#define Window_NoArgNoReturnFunction(X) \
static PyObject *PyCursesWindow_ ## X (PyCursesWindowObject *self, PyObject *args) \
{ if (!PyArg_NoArgs(args)) return NULL; \
  return PyCursesCheckERR(X(self->win), # X); }

#define Window_NoArgTrueFalseFunction(X) \
static PyObject * PyCursesWindow_ ## X (PyCursesWindowObject *self, PyObject *args) \
{ \
  if (!PyArg_NoArgs(args)) return NULL; \
  if (X (self->win) == FALSE) { Py_INCREF(Py_False); return Py_False; } \
  else { Py_INCREF(Py_True); return Py_True; } }

#define Window_NoArgNoReturnVoidFunction(X) \
static PyObject * PyCursesWindow_ ## X (PyCursesWindowObject *self, PyObject *args) \
{ \
  if (!PyArg_NoArgs(args)) return NULL; \
  X(self->win); Py_INCREF(Py_None); return Py_None; }

#define Window_NoArg2TupleReturnFunction(X, TYPE, ERGSTR) \
static PyObject * PyCursesWindow_ ## X (PyCursesWindowObject *self, PyObject *args) \
{ \
  TYPE arg1, arg2; \
  if (!PyArg_NoArgs(args)) return NULL; \
  X(self->win,arg1,arg2); return Py_BuildValue(ERGSTR, arg1, arg2); } 

#define Window_OneArgNoReturnVoidFunction(X, TYPE, PARSESTR) \
static PyObject * PyCursesWindow_ ## X (PyCursesWindowObject *self, PyObject *args) \
{ \
  TYPE arg1; \
  if (!PyArg_Parse(args, PARSESTR, &arg1)) return NULL; \
  X(self->win,arg1); Py_INCREF(Py_None); return Py_None; }

#define Window_OneArgNoReturnFunction(X, TYPE, PARSESTR) \
static PyObject * PyCursesWindow_ ## X (PyCursesWindowObject *self, PyObject *args) \
{ \
  TYPE arg1; \
  if (!PyArg_Parse(args,PARSESTR, &arg1)) return NULL; \
  return PyCursesCheckERR(X(self->win, arg1), # X); }

#define Window_TwoArgNoReturnFunction(X, TYPE, PARSESTR) \
static PyObject * PyCursesWindow_ ## X (PyCursesWindowObject *self, PyObject *args) \
{ \
  TYPE arg1, arg2; \
  if (!PyArg_Parse(args,PARSESTR, &arg1, &arg2)) return NULL; \
  return PyCursesCheckERR(X(self->win, arg1, arg2), # X); }

/* ------------- WINDOW routines --------------- */

Window_NoArgNoReturnFunction(untouchwin)
Window_NoArgNoReturnFunction(touchwin)
Window_NoArgNoReturnFunction(redrawwin)
Window_NoArgNoReturnFunction(winsertln)
Window_NoArgNoReturnFunction(werase)
Window_NoArgNoReturnFunction(wdeleteln)

Window_NoArgTrueFalseFunction(is_wintouched)

Window_NoArgNoReturnVoidFunction(wsyncup)
Window_NoArgNoReturnVoidFunction(wsyncdown)
Window_NoArgNoReturnVoidFunction(wstandend)
Window_NoArgNoReturnVoidFunction(wstandout)
Window_NoArgNoReturnVoidFunction(wcursyncup)
Window_NoArgNoReturnVoidFunction(wclrtoeol)
Window_NoArgNoReturnVoidFunction(wclrtobot)
Window_NoArgNoReturnVoidFunction(wclear)

Window_OneArgNoReturnVoidFunction(idcok, int, "i;True(1) or False(0)")
Window_OneArgNoReturnVoidFunction(immedok, int, "i;True(1) or False(0)")
Window_OneArgNoReturnVoidFunction(wtimeout, int, "i;delay")

Window_NoArg2TupleReturnFunction(getyx, int, "(ii)")
Window_NoArg2TupleReturnFunction(getbegyx, int, "(ii)")
Window_NoArg2TupleReturnFunction(getmaxyx, int, "(ii)")
Window_NoArg2TupleReturnFunction(getparyx, int, "(ii)")

Window_OneArgNoReturnFunction(wattron, attr_t, "l;attr")
Window_OneArgNoReturnFunction(wattroff, attr_t, "l;attr")
Window_OneArgNoReturnFunction(wattrset, attr_t, "l;attr")
Window_OneArgNoReturnFunction(clearok, int, "i;True(1) or False(0)")
Window_OneArgNoReturnFunction(idlok, int, "i;True(1) or False(0)")
#if defined(__NetBSD__)
Window_OneArgNoReturnVoidFunction(keypad, int, "i;True(1) or False(0)")
#else
Window_OneArgNoReturnFunction(keypad, int, "i;True(1) or False(0)")
#endif
Window_OneArgNoReturnFunction(leaveok, int, "i;True(1) or False(0)")
#if defined(__NetBSD__)
Window_OneArgNoReturnVoidFunction(nodelay, int, "i;True(1) or False(0)")
#else
Window_OneArgNoReturnFunction(nodelay, int, "i;True(1) or False(0)")
#endif
Window_OneArgNoReturnFunction(notimeout, int, "i;True(1) or False(0)")
Window_OneArgNoReturnFunction(scrollok, int, "i;True(1) or False(0)")
Window_OneArgNoReturnFunction(winsdelln, int, "i;nlines")
Window_OneArgNoReturnFunction(syncok, int, "i;True(1) or False(0)")

Window_TwoArgNoReturnFunction(mvwin, int, "(ii);y,x")
Window_TwoArgNoReturnFunction(mvderwin, int, "(ii);y,x")
Window_TwoArgNoReturnFunction(wmove, int, "(ii);y,x")
#ifndef STRICT_SYSV_CURSES
Window_TwoArgNoReturnFunction(wresize, int, "(ii);lines,columns")
#endif

/* Allocation and deallocation of Window Objects */

static PyObject *
PyCursesWindow_New(WINDOW *win)
{
	PyCursesWindowObject *wo;

	wo = PyObject_NEW(PyCursesWindowObject, &PyCursesWindow_Type);
	if (wo == NULL) return NULL;
	wo->win = win;
	return (PyObject *)wo;
}

static void
PyCursesWindow_Dealloc(PyCursesWindowObject *wo)
{
  if (wo->win != stdscr) delwin(wo->win);
  PyMem_DEL(wo);
}

/* Addch, Addstr, Addnstr */

static PyObject *
PyCursesWindow_AddCh(PyCursesWindowObject *self, PyObject *args)
{
  int rtn, x, y, use_xy = FALSE;
  PyObject *temp;
  chtype ch = 0;
  attr_t attr = A_NORMAL;
  
  switch (ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args, "O;ch or int", &temp))
	  return NULL;
    break;
  case 2:
    if (!PyArg_Parse(args, "(Ol);ch or int,attr", &temp, &attr))
      return NULL;
    break;
  case 3:
    if (!PyArg_Parse(args,"(iiO);y,x,ch or int", &y, &x, &temp))
      return NULL;
    use_xy = TRUE;
    break;
  case 4:
    if (!PyArg_Parse(args,"(iiOl);y,x,ch or int, attr", 
		     &y, &x, &temp, &attr))
      return NULL;
    use_xy = TRUE;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "addch requires 1 to 4 arguments");
    return NULL;
  }

  if (!PyCurses_ConvertToChtype(temp, &ch)) {
    PyErr_SetString(PyExc_TypeError, "argument 1 or 3 must be a ch or an int");
    return NULL;
  }
  
  if (use_xy == TRUE)
    rtn = mvwaddch(self->win,y,x, ch | attr);
  else {
    rtn = waddch(self->win, ch | attr);
  }
  return PyCursesCheckERR(rtn, "addch");
}

static PyObject *
PyCursesWindow_AddStr(PyCursesWindowObject *self, PyObject *args)
{
  int rtn;
  int x, y;
  char *str;
  attr_t attr = A_NORMAL , attr_old = A_NORMAL;
  int use_xy = FALSE, use_attr = FALSE;

  switch (ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args,"s;str", &str))
      return NULL;
    break;
  case 2:
    if (!PyArg_Parse(args,"(sl);str,attr", &str, &attr))
      return NULL;
    use_attr = TRUE;
    break;
  case 3:
    if (!PyArg_Parse(args,"(iis);int,int,str", &y, &x, &str))
      return NULL;
    use_xy = TRUE;
    break;
  case 4:
    if (!PyArg_Parse(args,"(iisl);int,int,str,attr", &y, &x, &str, &attr))
      return NULL;
    use_xy = use_attr = TRUE;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "addstr requires 1 to 4 arguments");
    return NULL;
  }

  if (use_attr == TRUE) {
    attr_old = getattrs(self->win);
    wattrset(self->win,attr);
  }
  if (use_xy == TRUE)
    rtn = mvwaddstr(self->win,y,x,str);
  else
    rtn = waddstr(self->win,str);
  if (use_attr == TRUE)
    wattrset(self->win,attr_old);
  return PyCursesCheckERR(rtn, "addstr");
}

static PyObject *
PyCursesWindow_AddNStr(PyCursesWindowObject *self, PyObject *args)
{
  int rtn, x, y, n;
  char *str;
  attr_t attr = A_NORMAL , attr_old = A_NORMAL;
  int use_xy = FALSE, use_attr = FALSE;

  switch (ARG_COUNT(args)) {
  case 2:
    if (!PyArg_Parse(args,"(si);str,n", &str, &n))
      return NULL;
    break;
  case 3:
    if (!PyArg_Parse(args,"(sil);str,n,attr", &str, &n, &attr))
      return NULL;
    use_attr = TRUE;
    break;
  case 4:
    if (!PyArg_Parse(args,"(iisi);y,x,str,n", &y, &x, &str, &n))
      return NULL;
    use_xy = TRUE;
    break;
  case 5:
    if (!PyArg_Parse(args,"(iisil);y,x,str,n,attr", &y, &x, &str, &n, &attr))
      return NULL;
    use_xy = use_attr = TRUE;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "addnstr requires 2 to 5 arguments");
    return NULL;
  }

  if (use_attr == TRUE) {
    attr_old = getattrs(self->win);
    wattrset(self->win,attr);
  }
  if (use_xy == TRUE)
    rtn = mvwaddnstr(self->win,y,x,str,n);
  else
    rtn = waddnstr(self->win,str,n);
  if (use_attr == TRUE)
    wattrset(self->win,attr_old);
  return PyCursesCheckERR(rtn, "addnstr");
}

static PyObject *
PyCursesWindow_Bkgd(PyCursesWindowObject *self, PyObject *args)
{
  PyObject *temp;
  chtype bkgd;
  attr_t attr = A_NORMAL;

  switch (ARG_COUNT(args)) {
    case 1:
      if (!PyArg_Parse(args, "O;ch or int", &temp))
        return NULL;
      break;
    case 2:
      if (!PyArg_Parse(args,"(Ol);ch or int,attr", &temp, &attr))
        return NULL;
      break;
    default:
      PyErr_SetString(PyExc_TypeError, "bkgd requires 1 or 2 arguments");
      return NULL;
  }

  if (!PyCurses_ConvertToChtype(temp, &bkgd)) {
    PyErr_SetString(PyExc_TypeError, "argument 1 or 3 must be a ch or an int");
    return NULL;
  }

  return PyCursesCheckERR(wbkgd(self->win, bkgd | attr), "bkgd");
}

static PyObject *
PyCursesWindow_BkgdSet(PyCursesWindowObject *self, PyObject *args)
{
  PyObject *temp;
  chtype bkgd;
  attr_t attr = A_NORMAL;

  switch (ARG_COUNT(args)) {
    case 1:
      if (!PyArg_Parse(args, "O;ch or int", &temp))
        return NULL;
      break;
    case 2:
      if (!PyArg_Parse(args,"(Ol);ch or int,attr", &temp, &attr))
        return NULL;
      break;
    default:
      PyErr_SetString(PyExc_TypeError, "bkgdset requires 1 or 2 arguments");
      return NULL;
  }

  if (!PyCurses_ConvertToChtype(temp, &bkgd)) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a ch or an int");
    return NULL;
  }

  wbkgdset(self->win, bkgd | attr);
  return PyCursesCheckERR(0, "bkgdset");
}

static PyObject *
PyCursesWindow_Border(PyCursesWindowObject *self, PyObject *args)
{
  PyObject *temp[8];
  chtype ch[8];
  int i;

  /* Clear the array of parameters */
  for(i=0; i<8; i++) {
       temp[i] = NULL;
       ch[i] = 0;
  }    
  
  if (!PyArg_ParseTuple(args,"|OOOOOOOO;ls,rs,ts,bs,tl,tr,bl,br",
                        &temp[0], &temp[1], &temp[2], &temp[3],
                        &temp[4], &temp[5], &temp[6], &temp[7]))
    return NULL;

  for(i=0; i<8; i++) {
      if (temp[i] != NULL && !PyCurses_ConvertToChtype(temp[i], &ch[i])) {
          PyErr_Format(PyExc_TypeError,
                       "argument %i must be a ch or an int", i+1);
          return NULL;
      }
  }
  
  wborder(self->win,
          ch[0], ch[1], ch[2], ch[3],
          ch[4], ch[5], ch[6], ch[7]);
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
PyCursesWindow_Box(PyCursesWindowObject *self, PyObject *args)
{
  chtype ch1=0,ch2=0;
  if (!PyArg_NoArgs(args)) {
    PyErr_Clear();
    if (!PyArg_Parse(args,"(ll);vertint,horint", &ch1, &ch2))
      return NULL;
  }
  box(self->win,ch1,ch2);
  Py_INCREF(Py_None);
  return Py_None;
}

#if defined(HAVE_NCURSES_H) || defined(MVWDELCH_IS_EXPRESSION)
#define py_mvwdelch mvwdelch
#else
int py_mvwdelch(WINDOW *w, int y, int x)
{
  mvwdelch(w,y,x);
  /* On HP/UX, mvwdelch already returns. On other systems,
     we may well run into this return statement. */
  return 0;
}
#endif


static PyObject *
PyCursesWindow_DelCh(PyCursesWindowObject *self, PyObject *args)
{
  int rtn;
  int x, y;

  switch (ARG_COUNT(args)) {
  case 0:
    rtn = wdelch(self->win);
    break;
  case 2:
    if (!PyArg_Parse(args,"(ii);y,x", &y, &x))
      return NULL;
    rtn = py_mvwdelch(self->win,y,x);
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "delch requires 0 or 2 arguments");
    return NULL;
  }
  return PyCursesCheckERR(rtn, "[mv]wdelch");
}

static PyObject *
PyCursesWindow_DerWin(PyCursesWindowObject *self, PyObject *args)
{
  WINDOW *win;
  int nlines, ncols, begin_y, begin_x;

  nlines = 0;
  ncols  = 0;
  switch (ARG_COUNT(args)) {
  case 2:
    if (!PyArg_Parse(args,"(ii);begin_y,begin_x",&begin_y,&begin_x))
      return NULL;
    break;
  case 4:
    if (!PyArg_Parse(args, "(iiii);nlines,ncols,begin_y,begin_x",
		   &nlines,&ncols,&begin_y,&begin_x))
      return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "derwin requires 2 or 4 arguments");
    return NULL;
  }

  win = derwin(self->win,nlines,ncols,begin_y,begin_x);

  if (win == NULL) {
    PyErr_SetString(PyCursesError, catchall_NULL);
    return NULL;
  }

  return (PyObject *)PyCursesWindow_New(win);
}

static PyObject *
PyCursesWindow_EchoChar(PyCursesWindowObject *self, PyObject *args)
{
  PyObject *temp;
  chtype ch;
  attr_t attr = A_NORMAL;

  switch (ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args,"O;ch or int", &temp))
      return NULL;
    break;
  case 2:
    if (!PyArg_Parse(args,"(Ol);ch or int,attr", &temp, &attr))
      return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "echochar requires 1 or 2 arguments");


    return NULL;
  }

  if (!PyCurses_ConvertToChtype(temp, &ch)) {
    PyErr_SetString(PyExc_TypeError, "argument 1 must be a ch or an int");
    return NULL;
  }
  
#ifdef WINDOW_HAS_FLAGS
  if (self->win->_flags & _ISPAD)
    return PyCursesCheckERR(pechochar(self->win, ch | attr), 
			    "echochar");
  else
#endif
    return PyCursesCheckERR(wechochar(self->win, ch | attr), 
			    "echochar");
}

#ifdef NCURSES_MOUSE_VERSION
static PyObject *
PyCursesWindow_Enclose(PyCursesWindowObject *self, PyObject *args)
{
	int x, y;
	if (!PyArg_Parse(args,"(ii);y,x", &y, &x))
		return NULL;

	return PyInt_FromLong( wenclose(self->win,y,x) );
}
#endif

static PyObject *
PyCursesWindow_GetBkgd(PyCursesWindowObject *self, PyObject *args)
{
  if (!PyArg_NoArgs(args))
    return NULL;
  return PyInt_FromLong((long) getbkgd(self->win));
}

static PyObject *
PyCursesWindow_GetCh(PyCursesWindowObject *self, PyObject *args)
{
  int x, y;
  chtype rtn;

  switch (ARG_COUNT(args)) {
  case 0:
    Py_BEGIN_ALLOW_THREADS
    rtn = wgetch(self->win);
    Py_END_ALLOW_THREADS
    break;
  case 2:
    if (!PyArg_Parse(args,"(ii);y,x",&y,&x))
      return NULL;
    Py_BEGIN_ALLOW_THREADS
    rtn = mvwgetch(self->win,y,x);
    Py_END_ALLOW_THREADS
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "getch requires 0 or 2 arguments");
    return NULL;
  }
  return PyInt_FromLong((long)rtn);
}

static PyObject *
PyCursesWindow_GetKey(PyCursesWindowObject *self, PyObject *args)
{
  int x, y;
  chtype rtn;

  switch (ARG_COUNT(args)) {
  case 0:
    Py_BEGIN_ALLOW_THREADS
    rtn = wgetch(self->win);
    Py_END_ALLOW_THREADS
    break;
  case 2:
    if (!PyArg_Parse(args,"(ii);y,x",&y,&x))
      return NULL;
    Py_BEGIN_ALLOW_THREADS
    rtn = mvwgetch(self->win,y,x);
    Py_END_ALLOW_THREADS
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "getkey requires 0 or 2 arguments");
    return NULL;
  }
  if (rtn<=255)
    return Py_BuildValue("c", rtn);
  else
#if defined(__NetBSD__)
    return PyString_FromString(unctrl(rtn));
#else
    return PyString_FromString((char *)keyname(rtn));
#endif
}

static PyObject *
PyCursesWindow_GetStr(PyCursesWindowObject *self, PyObject *args)
{
  int x, y, n;
  char rtn[1024]; /* This should be big enough.. I hope */
  int rtn2;

  switch (ARG_COUNT(args)) {
  case 0:
    Py_BEGIN_ALLOW_THREADS
    rtn2 = wgetstr(self->win,rtn);
    Py_END_ALLOW_THREADS
    break;
  case 1:
    if (!PyArg_Parse(args,"i;n", &n))
      return NULL;
    Py_BEGIN_ALLOW_THREADS
    rtn2 = wgetnstr(self->win,rtn,n);
    Py_END_ALLOW_THREADS
    break;
  case 2:
    if (!PyArg_Parse(args,"(ii);y,x",&y,&x))
      return NULL;
    Py_BEGIN_ALLOW_THREADS
    rtn2 = mvwgetstr(self->win,y,x,rtn);
    Py_END_ALLOW_THREADS
    break;
  case 3:
    if (!PyArg_Parse(args,"(iii);y,x,n", &y, &x, &n))
      return NULL;
#ifdef STRICT_SYSV_CURSES
 /* Untested */
    Py_BEGIN_ALLOW_THREADS
    rtn2 = wmove(self->win,y,x)==ERR ? ERR :
      wgetnstr(self->win, rtn, n);
    Py_END_ALLOW_THREADS
#else
    Py_BEGIN_ALLOW_THREADS
    rtn2 = mvwgetnstr(self->win, y, x, rtn, n);
    Py_END_ALLOW_THREADS
#endif
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "getstr requires 0 to 2 arguments");
    return NULL;
  }
  if (rtn2 == ERR)
    rtn[0] = 0;
  return PyString_FromString(rtn);
}

static PyObject *
PyCursesWindow_Hline(PyCursesWindowObject *self, PyObject *args)
{
  PyObject *temp;
  chtype ch;
  int n, x, y, code = OK;
  attr_t attr = A_NORMAL;

  switch (ARG_COUNT(args)) {
  case 2:
    if (!PyArg_Parse(args, "(Oi);ch or int,n", &temp, &n))
      return NULL;
    break;
  case 3:
    if (!PyArg_Parse(args, "(Oil);ch or int,n,attr", &temp, &n, &attr))
      return NULL;
    break;
  case 4:
    if (!PyArg_Parse(args, "(iiOi);y,x,ch or int,n", &y, &x, &temp, &n))
      return NULL;
    code = wmove(self->win, y, x);
    break;
  case 5:
    if (!PyArg_Parse(args, "(iiOil); y,x,ch or int,n,attr", 
		     &y, &x, &temp, &n, &attr))
      return NULL;
    code = wmove(self->win, y, x);
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "hline requires 2 to 5 arguments");
    return NULL;
  }

  if (code != ERR) {
    if (!PyCurses_ConvertToChtype(temp, &ch)) {
      PyErr_SetString(PyExc_TypeError, 
		      "argument 1 or 3 must be a ch or an int");
      return NULL;
    }
    return PyCursesCheckERR(whline(self->win, ch | attr, n), "hline");
  } else 
    return PyCursesCheckERR(code, "wmove");
}

static PyObject *
PyCursesWindow_InsCh(PyCursesWindowObject *self, PyObject *args)
{
  int rtn, x, y, use_xy = FALSE;
  PyObject *temp;
  chtype ch = 0;
  attr_t attr = A_NORMAL;
  
  switch (ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args, "O;ch or int", &temp))
      return NULL;
    break;
  case 2:
    if (!PyArg_Parse(args, "(Ol);ch or int,attr", &temp, &attr))
      return NULL;
    break;
  case 3:
    if (!PyArg_Parse(args,"(iiO);y,x,ch or int", &y, &x, &temp))
      return NULL;
    use_xy = TRUE;
    break;
  case 4:
    if (!PyArg_Parse(args,"(iiOl);y,x,ch or int, attr", &y, &x, &temp, &attr))
      return NULL;
    use_xy = TRUE;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "insch requires 1 or 4 arguments");
    return NULL;
  }

  if (!PyCurses_ConvertToChtype(temp, &ch)) {
    PyErr_SetString(PyExc_TypeError, 
		    "argument 1 or 3 must be a ch or an int");
    return NULL;
  }
  
  if (use_xy == TRUE)
    rtn = mvwinsch(self->win,y,x, ch | attr);
  else {
    rtn = winsch(self->win, ch | attr);
  }
  return PyCursesCheckERR(rtn, "insch");
}

static PyObject *
PyCursesWindow_InCh(PyCursesWindowObject *self, PyObject *args)
{
  int x, y, rtn;

  switch (ARG_COUNT(args)) {
  case 0:
    rtn = winch(self->win);
    break;
  case 2:
    if (!PyArg_Parse(args,"(ii);y,x",&y,&x))
      return NULL;
    rtn = mvwinch(self->win,y,x);
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "inch requires 0 or 2 arguments");
    return NULL;
  }
  return PyInt_FromLong((long) rtn);
}

static PyObject *
PyCursesWindow_InStr(PyCursesWindowObject *self, PyObject *args)
{
  int x, y, n;
  char rtn[1024]; /* This should be big enough.. I hope */
  int rtn2;

  switch (ARG_COUNT(args)) {
  case 0:
    rtn2 = winstr(self->win,rtn);
    break;
  case 1:
    if (!PyArg_Parse(args,"i;n", &n))
      return NULL;
    rtn2 = winnstr(self->win,rtn,n);
    break;
  case 2:
    if (!PyArg_Parse(args,"(ii);y,x",&y,&x))
      return NULL;
    rtn2 = mvwinstr(self->win,y,x,rtn);
    break;
  case 3:
    if (!PyArg_Parse(args, "(iii);y,x,n", &y, &x, &n))
      return NULL;
    rtn2 = mvwinnstr(self->win, y, x, rtn, n);
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "instr requires 0 or 3 arguments");
    return NULL;
  }
  if (rtn2 == ERR)
    rtn[0] = 0;
  return PyString_FromString(rtn);
}

static PyObject *
PyCursesWindow_InsStr(PyCursesWindowObject *self, PyObject *args)
{
  int rtn;
  int x, y;
  char *str;
  attr_t attr = A_NORMAL , attr_old = A_NORMAL;
  int use_xy = FALSE, use_attr = FALSE;

  switch (ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args,"s;str", &str))
      return NULL;
    break;
  case 2:
    if (!PyArg_Parse(args,"(sl);str,attr", &str, &attr))
      return NULL;
    use_attr = TRUE;
    break;
  case 3:
    if (!PyArg_Parse(args,"(iis);y,x,str", &y, &x, &str))
      return NULL;
    use_xy = TRUE;
    break;
  case 4:
    if (!PyArg_Parse(args,"(iisl);y,x,str,attr", &y, &x, &str, &attr))
      return NULL;
    use_xy = use_attr = TRUE;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "insstr requires 1 to 4 arguments");
    return NULL;
  }

  if (use_attr == TRUE) {
    attr_old = getattrs(self->win);
    wattrset(self->win,attr);
  }
  if (use_xy == TRUE)
    rtn = mvwinsstr(self->win,y,x,str);
  else
    rtn = winsstr(self->win,str);
  if (use_attr == TRUE)
    wattrset(self->win,attr_old);
  return PyCursesCheckERR(rtn, "insstr");
}

static PyObject *
PyCursesWindow_InsNStr(PyCursesWindowObject *self, PyObject *args)
{
  int rtn, x, y, n;
  char *str;
  attr_t attr = A_NORMAL , attr_old = A_NORMAL;
  int use_xy = FALSE, use_attr = FALSE;

  switch (ARG_COUNT(args)) {
  case 2:
    if (!PyArg_Parse(args,"(si);str,n", &str, &n))
      return NULL;
    break;
  case 3:
    if (!PyArg_Parse(args,"(sil);str,n,attr", &str, &n, &attr))
      return NULL;
    use_attr = TRUE;
    break;
  case 4:
    if (!PyArg_Parse(args,"(iisi);y,x,str,n", &y, &x, &str, &n))
      return NULL;
    use_xy = TRUE;
    break;
  case 5:
    if (!PyArg_Parse(args,"(iisil);y,x,str,n,attr", &y, &x, &str, &n, &attr))
      return NULL;
    use_xy = use_attr = TRUE;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "insnstr requires 2 to 5 arguments");
    return NULL;
  }

  if (use_attr == TRUE) {
    attr_old = getattrs(self->win);
    wattrset(self->win,attr);
  }
  if (use_xy == TRUE)
    rtn = mvwinsnstr(self->win,y,x,str,n);
  else
    rtn = winsnstr(self->win,str,n);
  if (use_attr == TRUE)
    wattrset(self->win,attr_old);
  return PyCursesCheckERR(rtn, "insnstr");
}

static PyObject *
PyCursesWindow_Is_LineTouched(PyCursesWindowObject *self, PyObject *args)
{
  int line, erg;
  if (!PyArg_Parse(args,"i;line", &line))
    return NULL;
  erg = is_linetouched(self->win, line);
  if (erg == ERR) {
    PyErr_SetString(PyExc_TypeError, 
		    "is_linetouched: line number outside of boundaries");
    return NULL;
  } else 
    if (erg == FALSE) {
      Py_INCREF(Py_False);
      return Py_False;
    } else {
      Py_INCREF(Py_True);
      return Py_True;
    }
}

static PyObject *
PyCursesWindow_NoOutRefresh(PyCursesWindowObject *self, PyObject *args)
{
  int pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol;
  int rtn;

#ifndef WINDOW_HAS_FLAGS
  if (0) {
#else
  if (self->win->_flags & _ISPAD) {
#endif
    switch(ARG_COUNT(args)) {
    case 6:
      if (!PyArg_Parse(args, 
		       "(iiiiii);" \
		       "pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol", 
		       &pminrow, &pmincol, &sminrow, 
		       &smincol, &smaxrow, &smaxcol))
	return NULL;
      Py_BEGIN_ALLOW_THREADS
      rtn = pnoutrefresh(self->win,
			 pminrow, pmincol, sminrow, 
			 smincol, smaxrow, smaxcol);
      Py_END_ALLOW_THREADS
      return PyCursesCheckERR(rtn, "pnoutrefresh");
    default:
      PyErr_SetString(PyCursesError, 
		      "noutrefresh() called for a pad " 
		      "requires 6 arguments");
      return NULL;
    }
  } else {
    if (!PyArg_NoArgs(args))
      return NULL;    

    Py_BEGIN_ALLOW_THREADS
    rtn = wnoutrefresh(self->win);
    Py_END_ALLOW_THREADS
    return PyCursesCheckERR(rtn, "wnoutrefresh");
  }
}

static PyObject *
PyCursesWindow_Overlay(PyCursesWindowObject *self, PyObject *args)
{
    PyCursesWindowObject *temp;
    int use_copywin = FALSE;
    int sminrow, smincol, dminrow, dmincol, dmaxrow, dmaxcol;
    int rtn;
    
    switch (ARG_COUNT(args)) {
    case 1:
	if (!PyArg_ParseTuple(args, "O!;window object",
			      &PyCursesWindow_Type, &temp))
	    return NULL;
	break;
    case 7:
	if (!PyArg_ParseTuple(args, "(O!iiiiii);window object, int, int, int, int, int, int",
			      &PyCursesWindow_Type, &temp, &sminrow, &smincol,
			      &dminrow, &dmincol, &dmaxrow, &dmaxcol))
	    return NULL;
	use_copywin = TRUE;
	break;
    default:
	PyErr_SetString(PyExc_TypeError,
			"overlay requires one or seven arguments");
	return NULL;
    }

    if (use_copywin == TRUE) {
	    rtn = copywin(self->win, temp->win, sminrow, smincol,
			  dminrow, dmincol, dmaxrow, dmaxcol, TRUE);
	    return PyCursesCheckERR(rtn, "copywin");
    }
    else {
	    rtn = overlay(self->win, temp->win);
	    return PyCursesCheckERR(rtn, "overlay");
    }
}

static PyObject *
PyCursesWindow_Overwrite(PyCursesWindowObject *self, PyObject *args)
{
    PyCursesWindowObject *temp;
    int use_copywin = FALSE;
    int sminrow, smincol, dminrow, dmincol, dmaxrow, dmaxcol;
    int rtn;
    
    switch (ARG_COUNT(args)) {
    case 1:
	if (!PyArg_ParseTuple(args, "O!;window object",
			      &PyCursesWindow_Type, &temp))
	    return NULL;
	break;
    case 7:
	if (!PyArg_ParseTuple(args, "(O!iiiiii);window object, int, int, int, int, int, int",
			      &PyCursesWindow_Type, &temp, &sminrow, &smincol,
			      &dminrow, &dmincol, &dmaxrow, &dmaxcol))
	    return NULL;
	use_copywin = TRUE;
	break;
    default:
	PyErr_SetString(PyExc_TypeError,
			"overwrite requires one or seven arguments");
	return NULL;
    }

    if (use_copywin == TRUE) {
	rtn = copywin(self->win, temp->win, sminrow, smincol,
		      dminrow, dmincol, dmaxrow, dmaxcol, FALSE);
        return PyCursesCheckERR(rtn, "copywin");
    }
    else {
	    rtn = overwrite(self->win, temp->win);
	    return PyCursesCheckERR(rtn, "overwrite");
    }
}

static PyObject *
PyCursesWindow_PutWin(PyCursesWindowObject *self, PyObject *args)
{
  PyObject *temp;
  
  if (!PyArg_Parse(args, "O;fileobj", &temp))
    return NULL;
  if (!PyFile_Check(temp)) {
    PyErr_SetString(PyExc_TypeError, "argument must be a file object");
    return NULL;
  }
  return PyCursesCheckERR(putwin(self->win, PyFile_AsFile(temp)), 
			  "putwin");
}

static PyObject *
PyCursesWindow_RedrawLine(PyCursesWindowObject *self, PyObject *args)
{
  int beg, num;
  if (!PyArg_Parse(args,"(ii);beg,num", &beg, &num))
    return NULL;
  return PyCursesCheckERR(wredrawln(self->win,beg,num), "redrawln");
}

static PyObject *
PyCursesWindow_Refresh(PyCursesWindowObject *self, PyObject *args)
{
  int pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol;
  int rtn;
  
#ifndef WINDOW_HAS_FLAGS
  if (0) {
#else
  if (self->win->_flags & _ISPAD) {
#endif
    switch(ARG_COUNT(args)) {
    case 6:
      if (!PyArg_Parse(args, 
		       "(iiiiii);" \
		       "pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol", 
		       &pminrow, &pmincol, &sminrow, 
		       &smincol, &smaxrow, &smaxcol))
	return NULL;

      Py_BEGIN_ALLOW_THREADS
      rtn = prefresh(self->win,
		     pminrow, pmincol, sminrow, 
		     smincol, smaxrow, smaxcol);
      Py_END_ALLOW_THREADS
      return PyCursesCheckERR(rtn, "prefresh");
    default:
      PyErr_SetString(PyCursesError, 
		      "refresh() for a pad requires 6 arguments");
      return NULL;
    }
  } else {
    if (!PyArg_NoArgs(args))
      return NULL;    
    Py_BEGIN_ALLOW_THREADS
    rtn = wrefresh(self->win);
    Py_END_ALLOW_THREADS
    return PyCursesCheckERR(rtn, "prefresh");    
  }
}

static PyObject *
PyCursesWindow_SetScrollRegion(PyCursesWindowObject *self, PyObject *args)
{
  int x, y;
  if (!PyArg_Parse(args,"(ii);top, bottom",&y,&x))
    return NULL;
  return PyCursesCheckERR(wsetscrreg(self->win,y,x), "wsetscrreg");
}

static PyObject *
PyCursesWindow_SubWin(PyCursesWindowObject *self, PyObject *args)
{
  WINDOW *win;
  int nlines, ncols, begin_y, begin_x;

  nlines = 0;
  ncols  = 0;
  switch (ARG_COUNT(args)) {
  case 2:
    if (!PyArg_Parse(args,"(ii);begin_y,begin_x",&begin_y,&begin_x))
      return NULL;
    break;
  case 4:
    if (!PyArg_Parse(args, "(iiii);nlines,ncols,begin_y,begin_x",
		   &nlines,&ncols,&begin_y,&begin_x))
      return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "subwin requires 2 or 4 arguments");
    return NULL;
  }

  /* printf("Subwin: %i %i %i %i   \n", nlines, ncols, begin_y, begin_x); */
#ifdef WINDOW_HAS_FLAGS
  if (self->win->_flags & _ISPAD)
    win = subpad(self->win, nlines, ncols, begin_y, begin_x);
  else
#endif
    win = subwin(self->win, nlines, ncols, begin_y, begin_x);

  if (win == NULL) {
    PyErr_SetString(PyCursesError, catchall_NULL);
    return NULL;
  }
  
  return (PyObject *)PyCursesWindow_New(win);
}

static PyObject *
PyCursesWindow_Scroll(PyCursesWindowObject *self, PyObject *args)
{
  int nlines;
  switch(ARG_COUNT(args)) {
  case 0:
    return PyCursesCheckERR(scroll(self->win), "scroll");
  case 1:
    if (!PyArg_Parse(args, "i;nlines", &nlines))
      return NULL;
    return PyCursesCheckERR(wscrl(self->win, nlines), "scroll");
  default:
    PyErr_SetString(PyExc_TypeError, "scroll requires 0 or 1 arguments");
    return NULL;
  }
}

static PyObject *
PyCursesWindow_TouchLine(PyCursesWindowObject *self, PyObject *args)
{
  int st, cnt, val;
  switch (ARG_COUNT(args)) {
  case 2:
    if (!PyArg_Parse(args,"(ii);start,count",&st,&cnt))
      return NULL;
    return PyCursesCheckERR(touchline(self->win,st,cnt), "touchline");
  case 3:
    if (!PyArg_Parse(args, "(iii);start,count,val", &st, &cnt, &val))
      return NULL;
    return PyCursesCheckERR(wtouchln(self->win, st, cnt, val), "touchline");
  default:
    PyErr_SetString(PyExc_TypeError, "touchline requires 2 or 3 arguments");
    return NULL;
  }
}

static PyObject *
PyCursesWindow_Vline(PyCursesWindowObject *self, PyObject *args)
{
  PyObject *temp;
  chtype ch;
  int n, x, y, code = OK;
  attr_t attr = A_NORMAL;

  switch (ARG_COUNT(args)) {
  case 2:
    if (!PyArg_Parse(args, "(Oi);ch or int,n", &temp, &n))
      return NULL;
    break;
  case 3:
    if (!PyArg_Parse(args, "(Oil);ch or int,n,attr", &temp, &n, &attr))
      return NULL;
    break;
  case 4:
    if (!PyArg_Parse(args, "(iiOi);y,x,ch or int,n", &y, &x, &temp, &n))
      return NULL;
    code = wmove(self->win, y, x);
    break;
  case 5:
    if (!PyArg_Parse(args, "(iiOil); y,x,ch or int,n,attr", 
		     &y, &x, &temp, &n, &attr))
      return NULL;
    code = wmove(self->win, y, x);
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "vline requires 2 to 5 arguments");
    return NULL;
  }

  if (code != ERR) {
    if (!PyCurses_ConvertToChtype(temp, &ch)) {
      PyErr_SetString(PyExc_TypeError, 
		      "argument 1 or 3 must be a ch or an int");
      return NULL;
    }
    return PyCursesCheckERR(wvline(self->win, ch | attr, n), "vline");
  } else
    return PyCursesCheckERR(code, "wmove");
}

static PyMethodDef PyCursesWindow_Methods[] = {
	{"addch",           (PyCFunction)PyCursesWindow_AddCh},
	{"addnstr",         (PyCFunction)PyCursesWindow_AddNStr},
	{"addstr",          (PyCFunction)PyCursesWindow_AddStr},
	{"attroff",         (PyCFunction)PyCursesWindow_wattroff},
	{"attron",          (PyCFunction)PyCursesWindow_wattron},
	{"attrset",         (PyCFunction)PyCursesWindow_wattrset},
	{"bkgd",            (PyCFunction)PyCursesWindow_Bkgd},
	{"bkgdset",         (PyCFunction)PyCursesWindow_BkgdSet},
	{"border",          (PyCFunction)PyCursesWindow_Border, METH_VARARGS},
	{"box",             (PyCFunction)PyCursesWindow_Box},
	{"clear",           (PyCFunction)PyCursesWindow_wclear},
	{"clearok",         (PyCFunction)PyCursesWindow_clearok},
	{"clrtobot",        (PyCFunction)PyCursesWindow_wclrtobot},
	{"clrtoeol",        (PyCFunction)PyCursesWindow_wclrtoeol},
	{"cursyncup",       (PyCFunction)PyCursesWindow_wcursyncup},
	{"delch",           (PyCFunction)PyCursesWindow_DelCh},
	{"deleteln",        (PyCFunction)PyCursesWindow_wdeleteln},
	{"derwin",          (PyCFunction)PyCursesWindow_DerWin},
	{"echochar",        (PyCFunction)PyCursesWindow_EchoChar},
#ifdef NCURSES_MOUSE_VERSION
	{"enclose",         (PyCFunction)PyCursesWindow_Enclose},
#endif
	{"erase",           (PyCFunction)PyCursesWindow_werase},
	{"getbegyx",        (PyCFunction)PyCursesWindow_getbegyx},
	{"getbkgd",         (PyCFunction)PyCursesWindow_GetBkgd},
	{"getch",           (PyCFunction)PyCursesWindow_GetCh},
	{"getkey",          (PyCFunction)PyCursesWindow_GetKey},
	{"getmaxyx",        (PyCFunction)PyCursesWindow_getmaxyx},
	{"getparyx",        (PyCFunction)PyCursesWindow_getparyx},
	{"getstr",          (PyCFunction)PyCursesWindow_GetStr},
	{"getyx",           (PyCFunction)PyCursesWindow_getyx},
	{"hline",           (PyCFunction)PyCursesWindow_Hline},
	{"idcok",           (PyCFunction)PyCursesWindow_idcok},
	{"idlok",           (PyCFunction)PyCursesWindow_idlok},
	{"immedok",         (PyCFunction)PyCursesWindow_immedok},
	{"inch",            (PyCFunction)PyCursesWindow_InCh},
	{"insch",           (PyCFunction)PyCursesWindow_InsCh},
	{"insdelln",        (PyCFunction)PyCursesWindow_winsdelln},
	{"insertln",        (PyCFunction)PyCursesWindow_winsertln},
	{"insnstr",         (PyCFunction)PyCursesWindow_InsNStr},
	{"insstr",          (PyCFunction)PyCursesWindow_InsStr},
	{"instr",           (PyCFunction)PyCursesWindow_InStr},
	{"is_linetouched",  (PyCFunction)PyCursesWindow_Is_LineTouched},
	{"is_wintouched",   (PyCFunction)PyCursesWindow_is_wintouched},
	{"keypad",          (PyCFunction)PyCursesWindow_keypad},
	{"leaveok",         (PyCFunction)PyCursesWindow_leaveok},
	{"move",            (PyCFunction)PyCursesWindow_wmove},
	{"mvderwin",        (PyCFunction)PyCursesWindow_mvderwin},
	{"mvwin",           (PyCFunction)PyCursesWindow_mvwin},
	{"nodelay",         (PyCFunction)PyCursesWindow_nodelay},
	{"notimeout",       (PyCFunction)PyCursesWindow_notimeout},
	{"noutrefresh",     (PyCFunction)PyCursesWindow_NoOutRefresh},
        /* Backward compatibility alias -- remove in Python 2.1 */
	{"nooutrefresh",    (PyCFunction)PyCursesWindow_NoOutRefresh},
	{"overlay",         (PyCFunction)PyCursesWindow_Overlay, METH_VARARGS},
	{"overwrite",       (PyCFunction)PyCursesWindow_Overwrite,
         METH_VARARGS},
	{"putwin",          (PyCFunction)PyCursesWindow_PutWin},
	{"redrawln",        (PyCFunction)PyCursesWindow_RedrawLine},
	{"redrawwin",       (PyCFunction)PyCursesWindow_redrawwin},
	{"refresh",         (PyCFunction)PyCursesWindow_Refresh},
#ifndef STRICT_SYSV_CURSES
	{"resize",          (PyCFunction)PyCursesWindow_wresize},
#endif
	{"scroll",          (PyCFunction)PyCursesWindow_Scroll},
	{"scrollok",        (PyCFunction)PyCursesWindow_scrollok},
	{"setscrreg",       (PyCFunction)PyCursesWindow_SetScrollRegion},
	{"standend",        (PyCFunction)PyCursesWindow_wstandend},
	{"standout",        (PyCFunction)PyCursesWindow_wstandout},
	{"subpad",          (PyCFunction)PyCursesWindow_SubWin},
	{"subwin",          (PyCFunction)PyCursesWindow_SubWin},
	{"syncdown",        (PyCFunction)PyCursesWindow_wsyncdown},
	{"syncok",          (PyCFunction)PyCursesWindow_syncok},
	{"syncup",          (PyCFunction)PyCursesWindow_wsyncup},
	{"timeout",         (PyCFunction)PyCursesWindow_wtimeout},
	{"touchline",       (PyCFunction)PyCursesWindow_TouchLine},
	{"touchwin",        (PyCFunction)PyCursesWindow_touchwin},
	{"untouchwin",      (PyCFunction)PyCursesWindow_untouchwin},
	{"vline",           (PyCFunction)PyCursesWindow_Vline},
	{NULL,		    NULL}   /* sentinel */
};

static PyObject *
PyCursesWindow_GetAttr(PyCursesWindowObject *self, char *name)
{
  return Py_FindMethod(PyCursesWindow_Methods, (PyObject *)self, name);
}

/* -------------------------------------------------------*/

PyTypeObject PyCursesWindow_Type = {
	PyObject_HEAD_INIT(NULL)
	0,			/*ob_size*/
	"_curses.curses window",	/*tp_name*/
	sizeof(PyCursesWindowObject),	/*tp_basicsize*/
	0,			/*tp_itemsize*/
	/* methods */
	(destructor)PyCursesWindow_Dealloc, /*tp_dealloc*/
	0,			/*tp_print*/
	(getattrfunc)PyCursesWindow_GetAttr, /*tp_getattr*/
	(setattrfunc)0, /*tp_setattr*/
	0,			/*tp_compare*/
	0,			/*tp_repr*/
	0,			/*tp_as_number*/
	0,			/*tp_as_sequence*/
	0,			/*tp_as_mapping*/
	0,			/*tp_hash*/
};

/*********************************************************************
 Global Functions
**********************************************************************/

NoArgNoReturnFunction(beep)
NoArgNoReturnFunction(def_prog_mode)
NoArgNoReturnFunction(def_shell_mode)
NoArgNoReturnFunction(doupdate)
NoArgNoReturnFunction(endwin)
NoArgNoReturnFunction(flash)
NoArgNoReturnFunction(nocbreak)
NoArgNoReturnFunction(noecho)
NoArgNoReturnFunction(nonl)
NoArgNoReturnFunction(noraw)
NoArgNoReturnFunction(reset_prog_mode)
NoArgNoReturnFunction(reset_shell_mode)
NoArgNoReturnFunction(resetty)
NoArgNoReturnFunction(savetty)

NoArgOrFlagNoReturnFunction(cbreak)
NoArgOrFlagNoReturnFunction(echo)
NoArgOrFlagNoReturnFunction(nl)
NoArgOrFlagNoReturnFunction(raw)

NoArgReturnIntFunction(baudrate)
NoArgReturnIntFunction(termattrs)

NoArgReturnStringFunction(termname)
NoArgReturnStringFunction(longname)

NoArgTrueFalseFunction(can_change_color)
NoArgTrueFalseFunction(has_colors)
NoArgTrueFalseFunction(has_ic)
NoArgTrueFalseFunction(has_il)
NoArgTrueFalseFunction(isendwin)
NoArgNoReturnVoidFunction(filter)
NoArgNoReturnVoidFunction(flushinp)
NoArgNoReturnVoidFunction(noqiflush)

static PyObject *
PyCurses_Color_Content(PyObject *self, PyObject *args)
{
  short color,r,g,b;

  PyCursesInitialised
  PyCursesInitialisedColor

  if (ARG_COUNT(args) != 1) {
    PyErr_SetString(PyExc_TypeError, 
		    "color_content requires 1 argument");
    return NULL;
  }

  if (!PyArg_Parse(args, "h;color", &color)) return NULL;

  if (color_content(color, &r, &g, &b) != ERR)
    return Py_BuildValue("(iii)", r, g, b);
  else {
    PyErr_SetString(PyCursesError, 
		    "Argument 1 was out of range. Check value of COLORS.");
    return NULL;
  }
}

static PyObject *
PyCurses_color_pair(PyObject *self, PyObject *args)
{
  int n;

  PyCursesInitialised
  PyCursesInitialisedColor

  if (ARG_COUNT(args) != 1) {
    PyErr_SetString(PyExc_TypeError, "color_pair requires 1 argument");
    return NULL;
  }
  if (!PyArg_Parse(args, "i;number", &n)) return NULL;
  return PyInt_FromLong((long) (n << 8));
}

static PyObject *
PyCurses_Curs_Set(PyObject *self, PyObject *args)
{
  int vis,erg;

  PyCursesInitialised

  if (ARG_COUNT(args)!=1) {
    PyErr_SetString(PyExc_TypeError, "curs_set requires 1 argument");
    return NULL;
  }

  if (!PyArg_Parse(args, "i;int", &vis)) return NULL;

  erg = curs_set(vis);
  if (erg == ERR) return PyCursesCheckERR(erg, "curs_set");

  return PyInt_FromLong((long) erg);
}

static PyObject *
PyCurses_Delay_Output(PyObject *self, PyObject *args)
{
  int ms;

  PyCursesInitialised

  if (ARG_COUNT(args) != 1) {
    PyErr_SetString(PyExc_TypeError, "delay_output requires 1 argument");
    return NULL;
  }
  if (!PyArg_Parse(args, "i;ms", &ms)) return NULL;

  return PyCursesCheckERR(delay_output(ms), "delay_output");
}

static PyObject *
PyCurses_EraseChar(PyObject *self, PyObject *args)
{
  char ch;

  PyCursesInitialised

  if (!PyArg_NoArgs(args)) return NULL;

  ch = erasechar();

  return PyString_FromStringAndSize(&ch, 1);
}

static PyObject *
PyCurses_getsyx(PyObject *self, PyObject *args)
{
  int x,y;

  PyCursesInitialised

  if (!PyArg_NoArgs(args)) return NULL;

  getsyx(y, x);

  return Py_BuildValue("(ii)", y, x);
}

#ifdef NCURSES_MOUSE_VERSION
static PyObject *
PyCurses_GetMouse(PyObject *self, PyObject *args)
{
	int rtn;
	MEVENT event;

	PyCursesInitialised
	if (!PyArg_NoArgs(args)) return NULL;

	rtn = getmouse( &event );
	if (rtn == ERR) {
		PyErr_SetString(PyCursesError, "getmouse() returned ERR");
		return NULL;
	}
	return Py_BuildValue("(hiiil)", 
			     (short)event.id, 
			     event.x, event.y, event.z,
			     (long) event.bstate);
}

static PyObject *
PyCurses_UngetMouse(PyObject *self, PyObject *args)
{
	MEVENT event;

	PyCursesInitialised
	if (!PyArg_ParseTuple(args, "(hiiil)",
			     &event.id, 
			     &event.x, &event.y, &event.z,
			     (int *) &event.bstate))
	  return NULL;

	return PyCursesCheckERR(ungetmouse(&event), "ungetmouse");
}
#endif

static PyObject *
PyCurses_GetWin(PyCursesWindowObject *self, PyObject *args)
{
  WINDOW *win;
  PyObject *temp;

  PyCursesInitialised

  if (!PyArg_Parse(args, "O;fileobj", &temp)) return NULL;

  if (!PyFile_Check(temp)) {
    PyErr_SetString(PyExc_TypeError, "argument must be a file object");
    return NULL;
  }

  win = getwin(PyFile_AsFile(temp));

  if (win == NULL) {
    PyErr_SetString(PyCursesError, catchall_NULL);
    return NULL;
  }

  return PyCursesWindow_New(win);
}

static PyObject *
PyCurses_HalfDelay(PyObject *self, PyObject *args)
{
  unsigned char tenths;

  PyCursesInitialised

  switch(ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args, "b;tenths", &tenths)) return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "halfdelay requires 1 argument");
    return NULL;
  }

  return PyCursesCheckERR(halfdelay(tenths), "halfdelay");
}

#ifndef STRICT_SYSV_CURSES
 /* No has_key! */
static PyObject * PyCurses_has_key(PyObject *self, PyObject *args)
{
  int ch;

  PyCursesInitialised

  if (!PyArg_Parse(args,"i",&ch)) return NULL;

  if (has_key(ch) == FALSE) {
    Py_INCREF(Py_False);
    return Py_False;
  }
  Py_INCREF(Py_True);
  return Py_True; 
}
#endif /* STRICT_SYSV_CURSES */

static PyObject *
PyCurses_Init_Color(PyObject *self, PyObject *args)
{
  short color, r, g, b;

  PyCursesInitialised
  PyCursesInitialisedColor

  switch(ARG_COUNT(args)) {
  case 4:
    if (!PyArg_Parse(args, "(hhhh);color,r,g,b", &color, &r, &g, &b)) return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "init_color requires 4 arguments");
    return NULL;
  }

  return PyCursesCheckERR(init_color(color, r, g, b), "init_color");
}

static PyObject *
PyCurses_Init_Pair(PyObject *self, PyObject *args)
{
  short pair, f, b;

  PyCursesInitialised
  PyCursesInitialisedColor

  if (ARG_COUNT(args) != 3) {
    PyErr_SetString(PyExc_TypeError, "init_pair requires 3 arguments");
    return NULL;
  }

  if (!PyArg_Parse(args, "(hhh);pair, f, b", &pair, &f, &b)) return NULL;

  return PyCursesCheckERR(init_pair(pair, f, b), "init_pair");
}

static PyObject *ModDict;

static PyObject * 
PyCurses_InitScr(PyObject *self, PyObject *args)
{
  WINDOW *win;
  PyObject *nlines, *cols;

  if (!PyArg_NoArgs(args)) return NULL;

  if (initialised == TRUE) {
    wrefresh(stdscr);
    return (PyObject *)PyCursesWindow_New(stdscr);
  }

  win = initscr();

  if (win == NULL) {
    PyErr_SetString(PyCursesError, catchall_NULL);
    return NULL;
  }

  initialised = initialised_setupterm = TRUE;

/* This was moved from initcurses() because it core dumped on SGI,
   where they're not defined until you've called initscr() */
#define SetDictInt(string,ch) \
	PyDict_SetItemString(ModDict,string,PyInt_FromLong((long) (ch)));

	/* Here are some graphic symbols you can use */
        SetDictInt("ACS_ULCORNER",      (ACS_ULCORNER));
	SetDictInt("ACS_LLCORNER",      (ACS_LLCORNER));
	SetDictInt("ACS_URCORNER",      (ACS_URCORNER));
	SetDictInt("ACS_LRCORNER",      (ACS_LRCORNER));
	SetDictInt("ACS_LTEE",          (ACS_LTEE));
	SetDictInt("ACS_RTEE",          (ACS_RTEE));
	SetDictInt("ACS_BTEE",          (ACS_BTEE));
	SetDictInt("ACS_TTEE",          (ACS_TTEE));
	SetDictInt("ACS_HLINE",         (ACS_HLINE));
	SetDictInt("ACS_VLINE",         (ACS_VLINE));
	SetDictInt("ACS_PLUS",          (ACS_PLUS));
#if !defined(__hpux) || defined(HAVE_NCURSES_H)
        /* On HP/UX 11, these are of type cchar_t, which is not an
           integral type. If this is a problem on more platforms, a
           configure test should be added to determine whether ACS_S1
           is of integral type. */
	SetDictInt("ACS_S1",            (ACS_S1));
	SetDictInt("ACS_S9",            (ACS_S9));
	SetDictInt("ACS_DIAMOND",       (ACS_DIAMOND));
	SetDictInt("ACS_CKBOARD",       (ACS_CKBOARD));
	SetDictInt("ACS_DEGREE",        (ACS_DEGREE));
	SetDictInt("ACS_PLMINUS",       (ACS_PLMINUS));
	SetDictInt("ACS_BULLET",        (ACS_BULLET));
	SetDictInt("ACS_LARROW",        (ACS_LARROW));
	SetDictInt("ACS_RARROW",        (ACS_RARROW));
	SetDictInt("ACS_DARROW",        (ACS_DARROW));
	SetDictInt("ACS_UARROW",        (ACS_UARROW));
	SetDictInt("ACS_BOARD",         (ACS_BOARD));
	SetDictInt("ACS_LANTERN",       (ACS_LANTERN));
	SetDictInt("ACS_BLOCK",         (ACS_BLOCK));
#endif
	SetDictInt("ACS_BSSB",          (ACS_ULCORNER));
	SetDictInt("ACS_SSBB",          (ACS_LLCORNER));
	SetDictInt("ACS_BBSS",          (ACS_URCORNER));
	SetDictInt("ACS_SBBS",          (ACS_LRCORNER));
	SetDictInt("ACS_SBSS",          (ACS_RTEE));
	SetDictInt("ACS_SSSB",          (ACS_LTEE));
	SetDictInt("ACS_SSBS",          (ACS_BTEE));
	SetDictInt("ACS_BSSS",          (ACS_TTEE));
	SetDictInt("ACS_BSBS",          (ACS_HLINE));
	SetDictInt("ACS_SBSB",          (ACS_VLINE));
	SetDictInt("ACS_SSSS",          (ACS_PLUS));

	/* The following are never available with strict SYSV curses */
#ifdef ACS_S3
	SetDictInt("ACS_S3",            (ACS_S3));
#endif
#ifdef ACS_S7
	SetDictInt("ACS_S7",            (ACS_S7));
#endif
#ifdef ACS_LEQUAL
	SetDictInt("ACS_LEQUAL",        (ACS_LEQUAL));
#endif
#ifdef ACS_GEQUAL
	SetDictInt("ACS_GEQUAL",        (ACS_GEQUAL));
#endif
#ifdef ACS_PI
	SetDictInt("ACS_PI",            (ACS_PI));
#endif
#ifdef ACS_NEQUAL
	SetDictInt("ACS_NEQUAL",        (ACS_NEQUAL));
#endif
#ifdef ACS_STERLING
	SetDictInt("ACS_STERLING",      (ACS_STERLING));
#endif

  nlines = PyInt_FromLong((long) LINES);
  PyDict_SetItemString(ModDict, "LINES", nlines);
  Py_DECREF(nlines);
  cols = PyInt_FromLong((long) COLS);
  PyDict_SetItemString(ModDict, "COLS", cols);
  Py_DECREF(cols);

  return (PyObject *)PyCursesWindow_New(win);
}

static PyObject *
PyCurses_setupterm(PyObject* self, PyObject *args, PyObject* keywds)
{
	int fd = -1;
	int err;
	char* termstr = NULL;

	static char *kwlist[] = {"term", "fd", NULL};

	if (!PyArg_ParseTupleAndKeywords(
		args,keywds,"|zi:setupterm",kwlist,&termstr,&fd)) {
		return NULL;
	}
	
	if (fd == -1) {
		PyObject* sys_stdout;

		sys_stdout = PySys_GetObject("stdout");

		if (sys_stdout == NULL) {
			PyErr_SetString(
				PyCursesError,
				"lost sys.stdout");
			return NULL;
		}

		fd = PyObject_AsFileDescriptor(sys_stdout);

		if (fd == -1) {
			return NULL;
		}
	}

	if (setupterm(termstr,fd,&err) == ERR) {
		char* s = "setupterm: unknown error";
		
		if (err == 0) {
			s = "setupterm: could not find terminal";
		} else if (err == -1) {
			s = "setupterm: could not find terminfo database";
		}

		PyErr_SetString(PyCursesError,s);
		return NULL;
	}

	initialised_setupterm = TRUE;

	Py_INCREF(Py_None);
	return Py_None;	
}

static PyObject *
PyCurses_IntrFlush(PyObject *self, PyObject *args)
{
  int ch;

  PyCursesInitialised

  switch(ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args,"i;True(1), False(0)",&ch)) return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "intrflush requires 1 argument");
    return NULL;
  }

  return PyCursesCheckERR(intrflush(NULL,ch), "intrflush");
}

#if !defined(__NetBSD__)
static PyObject *
PyCurses_KeyName(PyObject *self, PyObject *args)
{
  const char *knp;
  int ch;

  PyCursesInitialised

  if (!PyArg_Parse(args,"i",&ch)) return NULL;

  knp = keyname(ch);

  return PyString_FromString((knp == NULL) ? "" : (char *)knp);
}
#endif

static PyObject *  
PyCurses_KillChar(PyObject *self, PyObject *args)  
{  
  char ch;  

  if (!PyArg_NoArgs(args)) return NULL;  

  ch = killchar();  

  return PyString_FromStringAndSize(&ch, 1);  
}  

static PyObject *
PyCurses_Meta(PyObject *self, PyObject *args)
{
  int ch;

  PyCursesInitialised

  switch(ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args,"i;True(1), False(0)",&ch)) return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "meta requires 1 argument");
    return NULL;
  }

  return PyCursesCheckERR(meta(stdscr, ch), "meta");
}

#ifdef NCURSES_MOUSE_VERSION
static PyObject *
PyCurses_MouseInterval(PyObject *self, PyObject *args)
{
	int interval;
	PyCursesInitialised 

	if (!PyArg_Parse(args,"i;interval",&interval)) 
		return NULL;
	return PyCursesCheckERR(mouseinterval(interval), "mouseinterval");
}

static PyObject *
PyCurses_MouseMask(PyObject *self, PyObject *args)
{
	int newmask;
	mmask_t oldmask, availmask;

	PyCursesInitialised 
	if (!PyArg_Parse(args,"i;mousemask",&newmask)) 
		return NULL;
	availmask = mousemask(newmask, &oldmask);
	return Py_BuildValue("(ll)", (long)availmask, (long)oldmask);
}
#endif

static PyObject *
PyCurses_Napms(PyObject *self, PyObject *args)
{
    int ms;

    PyCursesInitialised
    if (!PyArg_Parse(args, "i;ms", &ms)) return NULL;

    return Py_BuildValue("i", napms(ms));
}


static PyObject *
PyCurses_NewPad(PyObject *self, PyObject *args)
{
  WINDOW *win;
  int nlines, ncols;

  PyCursesInitialised 

  if (!PyArg_Parse(args,"(ii);nlines,ncols",&nlines,&ncols)) return NULL;

  win = newpad(nlines, ncols);
  
  if (win == NULL) {
    PyErr_SetString(PyCursesError, catchall_NULL);
    return NULL;
  }

  return (PyObject *)PyCursesWindow_New(win);
}

static PyObject *
PyCurses_NewWindow(PyObject *self, PyObject *args)
{
  WINDOW *win;
  int nlines, ncols, begin_y=0, begin_x=0;

  PyCursesInitialised

  switch (ARG_COUNT(args)) {
  case 2:
    if (!PyArg_Parse(args,"(ii);nlines,ncols",&nlines,&ncols))
      return NULL;
    break;
  case 4:
    if (!PyArg_Parse(args, "(iiii);nlines,ncols,begin_y,begin_x",
		   &nlines,&ncols,&begin_y,&begin_x))
      return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "newwin requires 2 or 4 arguments");
    return NULL;
  }

  win = newwin(nlines,ncols,begin_y,begin_x);
  if (win == NULL) {
    PyErr_SetString(PyCursesError, catchall_NULL);
    return NULL;
  }

  return (PyObject *)PyCursesWindow_New(win);
}

static PyObject *
PyCurses_Pair_Content(PyObject *self, PyObject *args)
{
  short pair,f,b;

  PyCursesInitialised
  PyCursesInitialisedColor

  switch(ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args, "h;pair", &pair)) return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "pair_content requires 1 argument");
    return NULL;
  }

  if (!pair_content(pair, &f, &b)) {
    PyErr_SetString(PyCursesError,
		    "Argument 1 was out of range. (1..COLOR_PAIRS-1)");
    return NULL;
  }

  return Py_BuildValue("(ii)", f, b);
}

static PyObject *
PyCurses_pair_number(PyObject *self, PyObject *args)
{
  int n;

  PyCursesInitialised
  PyCursesInitialisedColor

  switch(ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args, "i;pairvalue", &n)) return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError,
                    "pair_number requires 1 argument");
    return NULL;
  }

  return PyInt_FromLong((long) ((n & A_COLOR) >> 8));
}

static PyObject *
PyCurses_Putp(PyObject *self, PyObject *args)
{
  char *str;

  if (!PyArg_Parse(args,"s;str", &str)) return NULL;
  return PyCursesCheckERR(putp(str), "putp");
}

static PyObject *
PyCurses_QiFlush(PyObject *self, PyObject *args)
{
  int flag = 0;

  PyCursesInitialised

  switch(ARG_COUNT(args)) {
  case 0:
    qiflush();
    Py_INCREF(Py_None);
    return Py_None;
  case 1:
    if (!PyArg_Parse(args, "i;True(1) or False(0)", &flag)) return NULL;
    if (flag) qiflush();
    else noqiflush();
    Py_INCREF(Py_None);
    return Py_None;
  default:
    PyErr_SetString(PyExc_TypeError, "qiflush requires 0 or 1 arguments");
    return NULL;
  }
}

static PyObject *
PyCurses_setsyx(PyObject *self, PyObject *args)
{
  int y,x;

  PyCursesInitialised

  if (ARG_COUNT(args)!=2) {
    PyErr_SetString(PyExc_TypeError, "setsyx requires 2 arguments");
    return NULL;
  }

  if (!PyArg_Parse(args, "(ii);y, x", &y, &x)) return NULL;

  setsyx(y,x);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
PyCurses_Start_Color(PyObject *self, PyObject *args)
{
  int code;
  PyObject *c, *cp;

  PyCursesInitialised

  if (!PyArg_NoArgs(args)) return NULL;

  code = start_color();
  if (code != ERR) {
    initialisedcolors = TRUE;
    c = PyInt_FromLong((long) COLORS);
    PyDict_SetItemString(ModDict, "COLORS", c);
    Py_DECREF(c);
    cp = PyInt_FromLong((long) COLOR_PAIRS);
    PyDict_SetItemString(ModDict, "COLOR_PAIRS", cp);
    Py_DECREF(cp);
    Py_INCREF(Py_None);
    return Py_None;
  } else {
    PyErr_SetString(PyCursesError, "start_color() returned ERR");
    return NULL;
  }
}

static PyObject *
PyCurses_tigetflag(PyObject *self, PyObject *args)
{
	char *capname;

	PyCursesSetupTermCalled;
		
	if (!PyArg_ParseTuple(args, "z", &capname))
		return NULL;

	return PyInt_FromLong( (long) tigetflag( capname ) );
}

static PyObject *
PyCurses_tigetnum(PyObject *self, PyObject *args)
{
	char *capname;

	PyCursesSetupTermCalled;
		
	if (!PyArg_ParseTuple(args, "z", &capname))
		return NULL;

	return PyInt_FromLong( (long) tigetnum( capname ) );
}

static PyObject *
PyCurses_tigetstr(PyObject *self, PyObject *args)
{
	char *capname;

	PyCursesSetupTermCalled;
		
	if (!PyArg_ParseTuple(args, "z", &capname))
		return NULL;

	capname = tigetstr( capname );
	if (capname == 0 || capname == (char*) -1) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	return PyString_FromString( capname );
}

static PyObject *
PyCurses_tparm(PyObject *self, PyObject *args)
{
	char* fmt;
	char* result = NULL;
	int i1,i2,i3,i4,i5,i6,i7,i8,i9;

	PyCursesSetupTermCalled;

	if (!PyArg_ParseTuple(args, "s|iiiiiiiii:tparm", 
			      &fmt, &i1, &i2, &i3, &i4, 
			      &i5, &i6, &i7, &i8, &i9)) {
		return NULL;
	}
	
#ifdef __hpux
        /* tparm is declared with 10 arguments on HP/UX 11.
           If this is a problem on other platforms as well,
           an autoconf test should be added to determine
           whether tparm can be called with a variable number
           of arguments. Perhaps the other arguments should
           be initialized in this case also. */
        result = tparm(fmt,i1,i2,i3,i4,i5,i6,i7,i8,i9);
#else
	switch (PyTuple_GET_SIZE(args)) {
	case 1:
		result = tparm(fmt);
		break;
	case 2:
		result = tparm(fmt,i1);
		break;
	case 3:
		result = tparm(fmt,i1,i2);
		break;
	case 4:
		result = tparm(fmt,i1,i2,i3);
		break;
	case 5:
		result = tparm(fmt,i1,i2,i3,i4);
		break;
	case 6:
		result = tparm(fmt,i1,i2,i3,i4,i5);
		break;
	case 7:
		result = tparm(fmt,i1,i2,i3,i4,i5,i6);
		break;
	case 8:
		result = tparm(fmt,i1,i2,i3,i4,i5,i6,i7);
		break;
	case 9:
		result = tparm(fmt,i1,i2,i3,i4,i5,i6,i7,i8);
		break;
	case 10:
		result = tparm(fmt,i1,i2,i3,i4,i5,i6,i7,i8,i9);
		break;
	}
#endif /* __hpux */
	return PyString_FromString(result);
}

static PyObject *
PyCurses_TypeAhead(PyObject *self, PyObject *args)
{
  int fd;

  PyCursesInitialised

  if (!PyArg_Parse(args,"i;fd",&fd)) return NULL;

  PyCursesCheckERR(typeahead( fd ), "typeahead");
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
PyCurses_UnCtrl(PyObject *self, PyObject *args)
{
  PyObject *temp;
  chtype ch;

  PyCursesInitialised

  if (!PyArg_Parse(args,"O;ch or int",&temp)) return NULL;

  if (PyInt_Check(temp))
    ch = (chtype) PyInt_AsLong(temp);
  else if (PyString_Check(temp))
    ch = (chtype) *PyString_AsString(temp);
  else {
    PyErr_SetString(PyExc_TypeError, "argument must be a ch or an int");
    return NULL;
  }

  return PyString_FromString(unctrl(ch));
}

static PyObject *
PyCurses_UngetCh(PyObject *self, PyObject *args)
{
  PyObject *temp;
  chtype ch;

  PyCursesInitialised

  if (!PyArg_Parse(args,"O;ch or int",&temp)) return NULL;

  if (PyInt_Check(temp))
    ch = (chtype) PyInt_AsLong(temp);
  else if (PyString_Check(temp))
    ch = (chtype) *PyString_AsString(temp);
  else {
    PyErr_SetString(PyExc_TypeError, "argument must be a ch or an int");
    return NULL;
  }

  return PyCursesCheckERR(ungetch(ch), "ungetch");
}

static PyObject *
PyCurses_Use_Env(PyObject *self, PyObject *args)
{
  int flag;

  PyCursesInitialised

  switch(ARG_COUNT(args)) {
  case 1:
    if (!PyArg_Parse(args,"i;True(1), False(0)",&flag))
      return NULL;
    break;
  default:
    PyErr_SetString(PyExc_TypeError, "use_env requires 1 argument");
    return NULL;
  }
  use_env(flag);
  Py_INCREF(Py_None);
  return Py_None;
}

/* List of functions defined in the module */

static PyMethodDef PyCurses_methods[] = {
  {"baudrate",            (PyCFunction)PyCurses_baudrate},
  {"beep",                (PyCFunction)PyCurses_beep},
  {"can_change_color",    (PyCFunction)PyCurses_can_change_color},
  {"cbreak",              (PyCFunction)PyCurses_cbreak},
  {"color_content",       (PyCFunction)PyCurses_Color_Content},
  {"color_pair",          (PyCFunction)PyCurses_color_pair},
  {"curs_set",            (PyCFunction)PyCurses_Curs_Set},
  {"def_prog_mode",       (PyCFunction)PyCurses_def_prog_mode},
  {"def_shell_mode",      (PyCFunction)PyCurses_def_shell_mode},
  {"delay_output",        (PyCFunction)PyCurses_Delay_Output},
  {"doupdate",            (PyCFunction)PyCurses_doupdate},
  {"echo",                (PyCFunction)PyCurses_echo},
  {"endwin",              (PyCFunction)PyCurses_endwin},
  {"erasechar",           (PyCFunction)PyCurses_EraseChar},
  {"filter",              (PyCFunction)PyCurses_filter},
  {"flash",               (PyCFunction)PyCurses_flash},
  {"flushinp",            (PyCFunction)PyCurses_flushinp},
#ifdef NCURSES_MOUSE_VERSION
  {"getmouse",            (PyCFunction)PyCurses_GetMouse},
  {"ungetmouse",          (PyCFunction)PyCurses_UngetMouse, METH_VARARGS},
#endif
  {"getsyx",              (PyCFunction)PyCurses_getsyx},
  {"getwin",              (PyCFunction)PyCurses_GetWin},
  {"has_colors",          (PyCFunction)PyCurses_has_colors},
  {"has_ic",              (PyCFunction)PyCurses_has_ic},
  {"has_il",              (PyCFunction)PyCurses_has_il},
#ifndef STRICT_SYSV_CURSES
  {"has_key",             (PyCFunction)PyCurses_has_key},
#endif
  {"halfdelay",           (PyCFunction)PyCurses_HalfDelay},
  {"init_color",          (PyCFunction)PyCurses_Init_Color},
  {"init_pair",           (PyCFunction)PyCurses_Init_Pair},
  {"initscr",             (PyCFunction)PyCurses_InitScr},
  {"intrflush",           (PyCFunction)PyCurses_IntrFlush},
  {"isendwin",            (PyCFunction)PyCurses_isendwin},
#if !defined(__NetBSD__)
  {"keyname",             (PyCFunction)PyCurses_KeyName},
#endif
  {"killchar",            (PyCFunction)PyCurses_KillChar}, 
  {"longname",            (PyCFunction)PyCurses_longname}, 
  {"meta",                (PyCFunction)PyCurses_Meta},
#ifdef NCURSES_MOUSE_VERSION
  {"mouseinterval",       (PyCFunction)PyCurses_MouseInterval},
  {"mousemask",           (PyCFunction)PyCurses_MouseMask},
#endif
  {"napms",               (PyCFunction)PyCurses_Napms},
  {"newpad",              (PyCFunction)PyCurses_NewPad},
  {"newwin",              (PyCFunction)PyCurses_NewWindow},
  {"nl",                  (PyCFunction)PyCurses_nl},
  {"nocbreak",            (PyCFunction)PyCurses_nocbreak},
  {"noecho",              (PyCFunction)PyCurses_noecho},
  {"nonl",                (PyCFunction)PyCurses_nonl},
  {"noqiflush",           (PyCFunction)PyCurses_noqiflush},
  {"noraw",               (PyCFunction)PyCurses_noraw},
  {"pair_content",        (PyCFunction)PyCurses_Pair_Content},
  {"pair_number",         (PyCFunction)PyCurses_pair_number},
  {"putp",                (PyCFunction)PyCurses_Putp},
  {"qiflush",             (PyCFunction)PyCurses_QiFlush},
  {"raw",                 (PyCFunction)PyCurses_raw},
  {"reset_prog_mode",     (PyCFunction)PyCurses_reset_prog_mode},
  {"reset_shell_mode",    (PyCFunction)PyCurses_reset_shell_mode},
  {"resetty",             (PyCFunction)PyCurses_resetty},
  {"savetty",             (PyCFunction)PyCurses_savetty},
  {"setsyx",              (PyCFunction)PyCurses_setsyx},
  {"setupterm",           (PyCFunction)PyCurses_setupterm,
   METH_VARARGS|METH_KEYWORDS},
  {"start_color",         (PyCFunction)PyCurses_Start_Color},
  {"termattrs",           (PyCFunction)PyCurses_termattrs},
  {"termname",            (PyCFunction)PyCurses_termname},
  {"tigetflag",		  (PyCFunction)PyCurses_tigetflag, METH_VARARGS},
  {"tigetnum",		  (PyCFunction)PyCurses_tigetnum, METH_VARARGS},
  {"tigetstr",		  (PyCFunction)PyCurses_tigetstr, METH_VARARGS},
  {"tparm",               (PyCFunction)PyCurses_tparm, METH_VARARGS},
  {"typeahead",           (PyCFunction)PyCurses_TypeAhead},
  {"unctrl",              (PyCFunction)PyCurses_UnCtrl},
  {"ungetch",             (PyCFunction)PyCurses_UngetCh},
  {"use_env",             (PyCFunction)PyCurses_Use_Env},
  {NULL,		  NULL}		/* sentinel */
};

/* Initialization function for the module */

DL_EXPORT(void)
init_curses(void)
{
	PyObject *m, *d, *v, *c_api_object;
	static void *PyCurses_API[PyCurses_API_pointers];

	/* Initialize object type */
	PyCursesWindow_Type.ob_type = &PyType_Type;

	/* Initialize the C API pointer array */
	PyCurses_API[0] = (void *)&PyCursesWindow_Type;
	PyCurses_API[1] = (void *)func_PyCursesSetupTermCalled;
	PyCurses_API[2] = (void *)func_PyCursesInitialised;
	PyCurses_API[3] = (void *)func_PyCursesInitialisedColor;

	/* Create the module and add the functions */
	m = Py_InitModule("_curses", PyCurses_methods);

	/* Add some symbolic constants to the module */
	d = PyModule_GetDict(m);
	ModDict = d; /* For PyCurses_InitScr to use later */

	/* Add a CObject for the C API */
	c_api_object = PyCObject_FromVoidPtr((void *)PyCurses_API, NULL);
	PyDict_SetItemString(d, "_C_API", c_api_object);
	Py_DECREF(c_api_object);

	/* For exception curses.error */
	PyCursesError = PyErr_NewException("_curses.error", NULL, NULL);
	PyDict_SetItemString(d, "error", PyCursesError);

	/* Make the version available */
	v = PyString_FromString(PyCursesVersion);
	PyDict_SetItemString(d, "version", v);
	PyDict_SetItemString(d, "__version__", v);
	Py_DECREF(v);

        SetDictInt("ERR", ERR);
        SetDictInt("OK", OK);

	/* Here are some attributes you can add to chars to print */
	
	SetDictInt("A_ATTRIBUTES",      A_ATTRIBUTES);
	SetDictInt("A_NORMAL",		A_NORMAL);
	SetDictInt("A_STANDOUT",	A_STANDOUT);
	SetDictInt("A_UNDERLINE",	A_UNDERLINE);
	SetDictInt("A_REVERSE",		A_REVERSE);
	SetDictInt("A_BLINK",		A_BLINK);
	SetDictInt("A_DIM",		A_DIM);
	SetDictInt("A_BOLD",		A_BOLD);
	SetDictInt("A_ALTCHARSET",	A_ALTCHARSET);
#if !defined(__NetBSD__)
	SetDictInt("A_INVIS",           A_INVIS);
#endif
	SetDictInt("A_PROTECT",         A_PROTECT);
	SetDictInt("A_CHARTEXT",        A_CHARTEXT);
	SetDictInt("A_COLOR",           A_COLOR);

	/* The following are never available with strict SYSV curses */
#ifdef A_HORIZONTAL
	SetDictInt("A_HORIZONTAL",      A_HORIZONTAL);
#endif
#ifdef A_LEFT
	SetDictInt("A_LEFT",            A_LEFT);
#endif
#ifdef A_LOW
	SetDictInt("A_LOW",             A_LOW);
#endif
#ifdef A_RIGHT
	SetDictInt("A_RIGHT",           A_RIGHT);
#endif
#ifdef A_TOP
	SetDictInt("A_TOP",             A_TOP);
#endif
#ifdef A_VERTICAL
	SetDictInt("A_VERTICAL",        A_VERTICAL);
#endif

	SetDictInt("COLOR_BLACK",       COLOR_BLACK);
	SetDictInt("COLOR_RED",         COLOR_RED);
	SetDictInt("COLOR_GREEN",       COLOR_GREEN);
	SetDictInt("COLOR_YELLOW",      COLOR_YELLOW);
	SetDictInt("COLOR_BLUE",        COLOR_BLUE);
	SetDictInt("COLOR_MAGENTA",     COLOR_MAGENTA);
	SetDictInt("COLOR_CYAN",        COLOR_CYAN);
	SetDictInt("COLOR_WHITE",       COLOR_WHITE);

#ifdef NCURSES_MOUSE_VERSION
	/* Mouse-related constants */
	SetDictInt("BUTTON1_PRESSED",          BUTTON1_PRESSED);
	SetDictInt("BUTTON1_RELEASED",         BUTTON1_RELEASED);
	SetDictInt("BUTTON1_CLICKED",          BUTTON1_CLICKED);
	SetDictInt("BUTTON1_DOUBLE_CLICKED",   BUTTON1_DOUBLE_CLICKED);
	SetDictInt("BUTTON1_TRIPLE_CLICKED",   BUTTON1_TRIPLE_CLICKED);

	SetDictInt("BUTTON2_PRESSED",          BUTTON2_PRESSED);
	SetDictInt("BUTTON2_RELEASED",         BUTTON2_RELEASED);
	SetDictInt("BUTTON2_CLICKED",          BUTTON2_CLICKED);
	SetDictInt("BUTTON2_DOUBLE_CLICKED",   BUTTON2_DOUBLE_CLICKED);
	SetDictInt("BUTTON2_TRIPLE_CLICKED",   BUTTON2_TRIPLE_CLICKED);

	SetDictInt("BUTTON3_PRESSED",          BUTTON3_PRESSED);
	SetDictInt("BUTTON3_RELEASED",         BUTTON3_RELEASED);
	SetDictInt("BUTTON3_CLICKED",          BUTTON3_CLICKED);
	SetDictInt("BUTTON3_DOUBLE_CLICKED",   BUTTON3_DOUBLE_CLICKED);
	SetDictInt("BUTTON3_TRIPLE_CLICKED",   BUTTON3_TRIPLE_CLICKED);

	SetDictInt("BUTTON4_PRESSED",          BUTTON4_PRESSED);
	SetDictInt("BUTTON4_RELEASED",         BUTTON4_RELEASED);
	SetDictInt("BUTTON4_CLICKED",          BUTTON4_CLICKED);
	SetDictInt("BUTTON4_DOUBLE_CLICKED",   BUTTON4_DOUBLE_CLICKED);
	SetDictInt("BUTTON4_TRIPLE_CLICKED",   BUTTON4_TRIPLE_CLICKED);

	SetDictInt("BUTTON_SHIFT",             BUTTON_SHIFT);
	SetDictInt("BUTTON_CTRL",              BUTTON_CTRL);
	SetDictInt("BUTTON_ALT",               BUTTON_ALT);

	SetDictInt("ALL_MOUSE_EVENTS",         ALL_MOUSE_EVENTS);
	SetDictInt("REPORT_MOUSE_POSITION",    REPORT_MOUSE_POSITION);
#endif
	/* Now set everything up for KEY_ variables */
	{
	  int key;
	  char *key_n;
	  char *key_n2;
#if !defined(__NetBSD__)
	  for (key=KEY_MIN;key < KEY_MAX; key++) {
	    key_n = (char *)keyname(key);
	    if (key_n == NULL || strcmp(key_n,"UNKNOWN KEY")==0)
	      continue;
	    if (strncmp(key_n,"KEY_F(",6)==0) {
	      char *p1, *p2;
	      key_n2 = malloc(strlen(key_n)+1);
	      p1 = key_n;
	      p2 = key_n2;
	      while (*p1) {
		if (*p1 != '(' && *p1 != ')') {
		  *p2 = *p1;
		  p2++;
		}
		p1++;
	      }
	      *p2 = (char)0;
	    } else
	      key_n2 = key_n;
	    PyDict_SetItemString(d,key_n2,PyInt_FromLong((long) key));
	    if (key_n2 != key_n)
	      free(key_n2);
	  }
#endif
	  SetDictInt("KEY_MIN", KEY_MIN);
	  SetDictInt("KEY_MAX", KEY_MAX);
	}
}
