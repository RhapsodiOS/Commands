
/* ========================== Module _Menu ========================== */

#include "Python.h"



#ifdef _WIN32
#include "pywintoolbox.h"
#else
#include "macglue.h"
#include "pymactoolbox.h"
#endif

/* Macro to test whether a weak-loaded CFM function exists */
#define PyMac_PRECHECK(rtn) do { if ( &rtn == NULL )  {\
    	PyErr_SetString(PyExc_NotImplementedError, \
    	"Not available in this shared library/OS version"); \
    	return NULL; \
    }} while(0)


#ifdef WITHOUT_FRAMEWORKS
#include <Devices.h> /* Defines OpenDeskAcc in universal headers */
#include <Menus.h>
#else
#include <Carbon/Carbon.h>
#endif


#ifdef USE_TOOLBOX_OBJECT_GLUE

extern PyObject *_MenuObj_New(MenuHandle);
extern int _MenuObj_Convert(PyObject *, MenuHandle *);

#define MenuObj_New _MenuObj_New
#define MenuObj_Convert _MenuObj_Convert 
#endif

#if !ACCESSOR_CALLS_ARE_FUNCTIONS
#define GetMenuID(menu) ((*(menu))->menuID)
#define GetMenuWidth(menu) ((*(menu))->menuWidth)
#define GetMenuHeight(menu) ((*(menu))->menuHeight)

#define SetMenuID(menu, id) ((*(menu))->menuID = (id))
#define SetMenuWidth(menu, width) ((*(menu))->menuWidth = (width))
#define SetMenuHeight(menu, height) ((*(menu))->menuHeight = (height))
#endif

#define as_Menu(h) ((MenuHandle)h)
#define as_Resource(h) ((Handle)h)

static PyObject *Menu_Error;

/* ------------------------ Object type Menu ------------------------ */

PyTypeObject Menu_Type;

#define MenuObj_Check(x) ((x)->ob_type == &Menu_Type)

typedef struct MenuObject {
	PyObject_HEAD
	MenuHandle ob_itself;
} MenuObject;

PyObject *MenuObj_New(MenuHandle itself)
{
	MenuObject *it;
	it = PyObject_NEW(MenuObject, &Menu_Type);
	if (it == NULL) return NULL;
	it->ob_itself = itself;
	return (PyObject *)it;
}
int MenuObj_Convert(PyObject *v, MenuHandle *p_itself)
{
	if (!MenuObj_Check(v))
	{
		PyErr_SetString(PyExc_TypeError, "Menu required");
		return 0;
	}
	*p_itself = ((MenuObject *)v)->ob_itself;
	return 1;
}

static void MenuObj_dealloc(MenuObject *self)
{
	/* Cleanup of self->ob_itself goes here */
	PyMem_DEL(self);
}

static PyObject *MenuObj_DisposeMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef DisposeMenu
	PyMac_PRECHECK(DisposeMenu);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	DisposeMenu(_self->ob_itself);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_CalcMenuSize(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef CalcMenuSize
	PyMac_PRECHECK(CalcMenuSize);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	CalcMenuSize(_self->ob_itself);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

#if !TARGET_API_MAC_CARBON

static PyObject *MenuObj_CountMItems(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short _rv;
#ifndef CountMItems
	PyMac_PRECHECK(CountMItems);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = CountMItems(_self->ob_itself);
	_res = Py_BuildValue("h",
	                     _rv);
	return _res;
}
#endif

static PyObject *MenuObj_CountMenuItems(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short _rv;
#ifndef CountMenuItems
	PyMac_PRECHECK(CountMenuItems);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = CountMenuItems(_self->ob_itself);
	_res = Py_BuildValue("h",
	                     _rv);
	return _res;
}

static PyObject *MenuObj_GetMenuFont(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	SInt16 outFontID;
	UInt16 outFontSize;
#ifndef GetMenuFont
	PyMac_PRECHECK(GetMenuFont);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = GetMenuFont(_self->ob_itself,
	                   &outFontID,
	                   &outFontSize);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("hH",
	                     outFontID,
	                     outFontSize);
	return _res;
}

static PyObject *MenuObj_SetMenuFont(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	SInt16 inFontID;
	UInt16 inFontSize;
#ifndef SetMenuFont
	PyMac_PRECHECK(SetMenuFont);
#endif
	if (!PyArg_ParseTuple(_args, "hH",
	                      &inFontID,
	                      &inFontSize))
		return NULL;
	_err = SetMenuFont(_self->ob_itself,
	                   inFontID,
	                   inFontSize);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuExcludesMarkColumn(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
#ifndef GetMenuExcludesMarkColumn
	PyMac_PRECHECK(GetMenuExcludesMarkColumn);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = GetMenuExcludesMarkColumn(_self->ob_itself);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}

static PyObject *MenuObj_SetMenuExcludesMarkColumn(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	Boolean excludesMark;
#ifndef SetMenuExcludesMarkColumn
	PyMac_PRECHECK(SetMenuExcludesMarkColumn);
#endif
	if (!PyArg_ParseTuple(_args, "b",
	                      &excludesMark))
		return NULL;
	_err = SetMenuExcludesMarkColumn(_self->ob_itself,
	                                 excludesMark);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_IsValidMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
#ifndef IsValidMenu
	PyMac_PRECHECK(IsValidMenu);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = IsValidMenu(_self->ob_itself);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuRetainCount(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	ItemCount _rv;
#ifndef GetMenuRetainCount
	PyMac_PRECHECK(GetMenuRetainCount);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = GetMenuRetainCount(_self->ob_itself);
	_res = Py_BuildValue("l",
	                     _rv);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_RetainMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
#ifndef RetainMenu
	PyMac_PRECHECK(RetainMenu);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = RetainMenu(_self->ob_itself);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_ReleaseMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
#ifndef ReleaseMenu
	PyMac_PRECHECK(ReleaseMenu);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = ReleaseMenu(_self->ob_itself);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_DuplicateMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuHandle outMenu;
#ifndef DuplicateMenu
	PyMac_PRECHECK(DuplicateMenu);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = DuplicateMenu(_self->ob_itself,
	                     &outMenu);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("O&",
	                     MenuObj_New, outMenu);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_CopyMenuTitleAsCFString(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	CFStringRef outString;
#ifndef CopyMenuTitleAsCFString
	PyMac_PRECHECK(CopyMenuTitleAsCFString);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = CopyMenuTitleAsCFString(_self->ob_itself,
	                               &outString);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("O&",
	                     CFStringRefObj_New, outString);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_SetMenuTitleWithCFString(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	CFStringRef inString;
#ifndef SetMenuTitleWithCFString
	PyMac_PRECHECK(SetMenuTitleWithCFString);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      CFStringRefObj_Convert, &inString))
		return NULL;
	_err = SetMenuTitleWithCFString(_self->ob_itself,
	                                inString);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_InvalidateMenuSize(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
#ifndef InvalidateMenuSize
	PyMac_PRECHECK(InvalidateMenuSize);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = InvalidateMenuSize(_self->ob_itself);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_IsMenuSizeInvalid(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
#ifndef IsMenuSizeInvalid
	PyMac_PRECHECK(IsMenuSizeInvalid);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = IsMenuSizeInvalid(_self->ob_itself);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}
#endif

static PyObject *MenuObj_MacAppendMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Str255 data;
#ifndef MacAppendMenu
	PyMac_PRECHECK(MacAppendMenu);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      PyMac_GetStr255, data))
		return NULL;
	MacAppendMenu(_self->ob_itself,
	              data);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_InsertResMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	ResType theType;
	short afterItem;
#ifndef InsertResMenu
	PyMac_PRECHECK(InsertResMenu);
#endif
	if (!PyArg_ParseTuple(_args, "O&h",
	                      PyMac_GetOSType, &theType,
	                      &afterItem))
		return NULL;
	InsertResMenu(_self->ob_itself,
	              theType,
	              afterItem);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_AppendResMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	ResType theType;
#ifndef AppendResMenu
	PyMac_PRECHECK(AppendResMenu);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      PyMac_GetOSType, &theType))
		return NULL;
	AppendResMenu(_self->ob_itself,
	              theType);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_MacInsertMenuItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Str255 itemString;
	short afterItem;
#ifndef MacInsertMenuItem
	PyMac_PRECHECK(MacInsertMenuItem);
#endif
	if (!PyArg_ParseTuple(_args, "O&h",
	                      PyMac_GetStr255, itemString,
	                      &afterItem))
		return NULL;
	MacInsertMenuItem(_self->ob_itself,
	                  itemString,
	                  afterItem);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_DeleteMenuItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
#ifndef DeleteMenuItem
	PyMac_PRECHECK(DeleteMenuItem);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	DeleteMenuItem(_self->ob_itself,
	               item);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_InsertFontResMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short afterItem;
	short scriptFilter;
#ifndef InsertFontResMenu
	PyMac_PRECHECK(InsertFontResMenu);
#endif
	if (!PyArg_ParseTuple(_args, "hh",
	                      &afterItem,
	                      &scriptFilter))
		return NULL;
	InsertFontResMenu(_self->ob_itself,
	                  afterItem,
	                  scriptFilter);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_InsertIntlResMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	ResType theType;
	short afterItem;
	short scriptFilter;
#ifndef InsertIntlResMenu
	PyMac_PRECHECK(InsertIntlResMenu);
#endif
	if (!PyArg_ParseTuple(_args, "O&hh",
	                      PyMac_GetOSType, &theType,
	                      &afterItem,
	                      &scriptFilter))
		return NULL;
	InsertIntlResMenu(_self->ob_itself,
	                  theType,
	                  afterItem,
	                  scriptFilter);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_AppendMenuItemText(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	Str255 inString;
#ifndef AppendMenuItemText
	PyMac_PRECHECK(AppendMenuItemText);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      PyMac_GetStr255, inString))
		return NULL;
	_err = AppendMenuItemText(_self->ob_itself,
	                          inString);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_InsertMenuItemText(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	Str255 inString;
	MenuItemIndex afterItem;
#ifndef InsertMenuItemText
	PyMac_PRECHECK(InsertMenuItemText);
#endif
	if (!PyArg_ParseTuple(_args, "O&h",
	                      PyMac_GetStr255, inString,
	                      &afterItem))
		return NULL;
	_err = InsertMenuItemText(_self->ob_itself,
	                          inString,
	                          afterItem);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_CopyMenuItems(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inFirstItem;
	ItemCount inNumItems;
	MenuHandle inDestMenu;
	MenuItemIndex inInsertAfter;
#ifndef CopyMenuItems
	PyMac_PRECHECK(CopyMenuItems);
#endif
	if (!PyArg_ParseTuple(_args, "hlO&h",
	                      &inFirstItem,
	                      &inNumItems,
	                      MenuObj_Convert, &inDestMenu,
	                      &inInsertAfter))
		return NULL;
	_err = CopyMenuItems(_self->ob_itself,
	                     inFirstItem,
	                     inNumItems,
	                     inDestMenu,
	                     inInsertAfter);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_DeleteMenuItems(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inFirstItem;
	ItemCount inNumItems;
#ifndef DeleteMenuItems
	PyMac_PRECHECK(DeleteMenuItems);
#endif
	if (!PyArg_ParseTuple(_args, "hl",
	                      &inFirstItem,
	                      &inNumItems))
		return NULL;
	_err = DeleteMenuItems(_self->ob_itself,
	                       inFirstItem,
	                       inNumItems);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_AppendMenuItemTextWithCFString(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	CFStringRef inString;
	MenuItemAttributes inAttributes;
	MenuCommand inCommandID;
	MenuItemIndex outNewItem;
#ifndef AppendMenuItemTextWithCFString
	PyMac_PRECHECK(AppendMenuItemTextWithCFString);
#endif
	if (!PyArg_ParseTuple(_args, "O&ll",
	                      CFStringRefObj_Convert, &inString,
	                      &inAttributes,
	                      &inCommandID))
		return NULL;
	_err = AppendMenuItemTextWithCFString(_self->ob_itself,
	                                      inString,
	                                      inAttributes,
	                                      inCommandID,
	                                      &outNewItem);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("h",
	                     outNewItem);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_InsertMenuItemTextWithCFString(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	CFStringRef inString;
	MenuItemIndex inAfterItem;
	MenuItemAttributes inAttributes;
	MenuCommand inCommandID;
#ifndef InsertMenuItemTextWithCFString
	PyMac_PRECHECK(InsertMenuItemTextWithCFString);
#endif
	if (!PyArg_ParseTuple(_args, "O&hll",
	                      CFStringRefObj_Convert, &inString,
	                      &inAfterItem,
	                      &inAttributes,
	                      &inCommandID))
		return NULL;
	_err = InsertMenuItemTextWithCFString(_self->ob_itself,
	                                      inString,
	                                      inAfterItem,
	                                      inAttributes,
	                                      inCommandID);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

static PyObject *MenuObj_PopUpMenuSelect(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	long _rv;
	short top;
	short left;
	short popUpItem;
#ifndef PopUpMenuSelect
	PyMac_PRECHECK(PopUpMenuSelect);
#endif
	if (!PyArg_ParseTuple(_args, "hhh",
	                      &top,
	                      &left,
	                      &popUpItem))
		return NULL;
	_rv = PopUpMenuSelect(_self->ob_itself,
	                      top,
	                      left,
	                      popUpItem);
	_res = Py_BuildValue("l",
	                     _rv);
	return _res;
}

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_InvalidateMenuEnabling(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
#ifndef InvalidateMenuEnabling
	PyMac_PRECHECK(InvalidateMenuEnabling);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = InvalidateMenuEnabling(_self->ob_itself);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_IsMenuBarInvalid(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
#ifndef IsMenuBarInvalid
	PyMac_PRECHECK(IsMenuBarInvalid);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = IsMenuBarInvalid(_self->ob_itself);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}
#endif

static PyObject *MenuObj_MacInsertMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuID beforeID;
#ifndef MacInsertMenu
	PyMac_PRECHECK(MacInsertMenu);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &beforeID))
		return NULL;
	MacInsertMenu(_self->ob_itself,
	              beforeID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_SetRootMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
#ifndef SetRootMenu
	PyMac_PRECHECK(SetRootMenu);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = SetRootMenu(_self->ob_itself);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if !TARGET_API_MAC_CARBON

static PyObject *MenuObj_CheckItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	Boolean checked;
#ifndef CheckItem
	PyMac_PRECHECK(CheckItem);
#endif
	if (!PyArg_ParseTuple(_args, "hb",
	                      &item,
	                      &checked))
		return NULL;
	CheckItem(_self->ob_itself,
	          item,
	          checked);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

static PyObject *MenuObj_MacCheckMenuItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	Boolean checked;
#ifndef MacCheckMenuItem
	PyMac_PRECHECK(MacCheckMenuItem);
#endif
	if (!PyArg_ParseTuple(_args, "hb",
	                      &item,
	                      &checked))
		return NULL;
	MacCheckMenuItem(_self->ob_itself,
	                 item,
	                 checked);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_SetMenuItemText(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	Str255 itemString;
#ifndef SetMenuItemText
	PyMac_PRECHECK(SetMenuItemText);
#endif
	if (!PyArg_ParseTuple(_args, "hO&",
	                      &item,
	                      PyMac_GetStr255, itemString))
		return NULL;
	SetMenuItemText(_self->ob_itself,
	                item,
	                itemString);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuItemText(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	Str255 itemString;
#ifndef GetMenuItemText
	PyMac_PRECHECK(GetMenuItemText);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	GetMenuItemText(_self->ob_itself,
	                item,
	                itemString);
	_res = Py_BuildValue("O&",
	                     PyMac_BuildStr255, itemString);
	return _res;
}

static PyObject *MenuObj_SetItemMark(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	CharParameter markChar;
#ifndef SetItemMark
	PyMac_PRECHECK(SetItemMark);
#endif
	if (!PyArg_ParseTuple(_args, "hh",
	                      &item,
	                      &markChar))
		return NULL;
	SetItemMark(_self->ob_itself,
	            item,
	            markChar);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetItemMark(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	CharParameter markChar;
#ifndef GetItemMark
	PyMac_PRECHECK(GetItemMark);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	GetItemMark(_self->ob_itself,
	            item,
	            &markChar);
	_res = Py_BuildValue("h",
	                     markChar);
	return _res;
}

static PyObject *MenuObj_SetItemCmd(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	CharParameter cmdChar;
#ifndef SetItemCmd
	PyMac_PRECHECK(SetItemCmd);
#endif
	if (!PyArg_ParseTuple(_args, "hh",
	                      &item,
	                      &cmdChar))
		return NULL;
	SetItemCmd(_self->ob_itself,
	           item,
	           cmdChar);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetItemCmd(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	CharParameter cmdChar;
#ifndef GetItemCmd
	PyMac_PRECHECK(GetItemCmd);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	GetItemCmd(_self->ob_itself,
	           item,
	           &cmdChar);
	_res = Py_BuildValue("h",
	                     cmdChar);
	return _res;
}

static PyObject *MenuObj_SetItemIcon(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	short iconIndex;
#ifndef SetItemIcon
	PyMac_PRECHECK(SetItemIcon);
#endif
	if (!PyArg_ParseTuple(_args, "hh",
	                      &item,
	                      &iconIndex))
		return NULL;
	SetItemIcon(_self->ob_itself,
	            item,
	            iconIndex);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetItemIcon(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	short iconIndex;
#ifndef GetItemIcon
	PyMac_PRECHECK(GetItemIcon);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	GetItemIcon(_self->ob_itself,
	            item,
	            &iconIndex);
	_res = Py_BuildValue("h",
	                     iconIndex);
	return _res;
}

static PyObject *MenuObj_SetItemStyle(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	StyleParameter chStyle;
#ifndef SetItemStyle
	PyMac_PRECHECK(SetItemStyle);
#endif
	if (!PyArg_ParseTuple(_args, "hh",
	                      &item,
	                      &chStyle))
		return NULL;
	SetItemStyle(_self->ob_itself,
	             item,
	             chStyle);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetItemStyle(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	Style chStyle;
#ifndef GetItemStyle
	PyMac_PRECHECK(GetItemStyle);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	GetItemStyle(_self->ob_itself,
	             item,
	             &chStyle);
	_res = Py_BuildValue("b",
	                     chStyle);
	return _res;
}

#if !TARGET_API_MAC_CARBON

static PyObject *MenuObj_DisableItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
#ifndef DisableItem
	PyMac_PRECHECK(DisableItem);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	DisableItem(_self->ob_itself,
	            item);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if !TARGET_API_MAC_CARBON

static PyObject *MenuObj_EnableItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
#ifndef EnableItem
	PyMac_PRECHECK(EnableItem);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	EnableItem(_self->ob_itself,
	           item);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

static PyObject *MenuObj_SetMenuItemCommandID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	MenuCommand inCommandID;
#ifndef SetMenuItemCommandID
	PyMac_PRECHECK(SetMenuItemCommandID);
#endif
	if (!PyArg_ParseTuple(_args, "hl",
	                      &inItem,
	                      &inCommandID))
		return NULL;
	_err = SetMenuItemCommandID(_self->ob_itself,
	                            inItem,
	                            inCommandID);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuItemCommandID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	MenuCommand outCommandID;
#ifndef GetMenuItemCommandID
	PyMac_PRECHECK(GetMenuItemCommandID);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemCommandID(_self->ob_itself,
	                            inItem,
	                            &outCommandID);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outCommandID);
	return _res;
}

static PyObject *MenuObj_SetMenuItemModifiers(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	UInt8 inModifiers;
#ifndef SetMenuItemModifiers
	PyMac_PRECHECK(SetMenuItemModifiers);
#endif
	if (!PyArg_ParseTuple(_args, "hb",
	                      &inItem,
	                      &inModifiers))
		return NULL;
	_err = SetMenuItemModifiers(_self->ob_itself,
	                            inItem,
	                            inModifiers);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuItemModifiers(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	UInt8 outModifiers;
#ifndef GetMenuItemModifiers
	PyMac_PRECHECK(GetMenuItemModifiers);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemModifiers(_self->ob_itself,
	                            inItem,
	                            &outModifiers);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("b",
	                     outModifiers);
	return _res;
}

static PyObject *MenuObj_SetMenuItemIconHandle(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	UInt8 inIconType;
	Handle inIconHandle;
#ifndef SetMenuItemIconHandle
	PyMac_PRECHECK(SetMenuItemIconHandle);
#endif
	if (!PyArg_ParseTuple(_args, "hbO&",
	                      &inItem,
	                      &inIconType,
	                      ResObj_Convert, &inIconHandle))
		return NULL;
	_err = SetMenuItemIconHandle(_self->ob_itself,
	                             inItem,
	                             inIconType,
	                             inIconHandle);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuItemIconHandle(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	UInt8 outIconType;
	Handle outIconHandle;
#ifndef GetMenuItemIconHandle
	PyMac_PRECHECK(GetMenuItemIconHandle);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemIconHandle(_self->ob_itself,
	                             inItem,
	                             &outIconType,
	                             &outIconHandle);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("bO&",
	                     outIconType,
	                     ResObj_New, outIconHandle);
	return _res;
}

static PyObject *MenuObj_SetMenuItemTextEncoding(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	TextEncoding inScriptID;
#ifndef SetMenuItemTextEncoding
	PyMac_PRECHECK(SetMenuItemTextEncoding);
#endif
	if (!PyArg_ParseTuple(_args, "hl",
	                      &inItem,
	                      &inScriptID))
		return NULL;
	_err = SetMenuItemTextEncoding(_self->ob_itself,
	                               inItem,
	                               inScriptID);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuItemTextEncoding(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	TextEncoding outScriptID;
#ifndef GetMenuItemTextEncoding
	PyMac_PRECHECK(GetMenuItemTextEncoding);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemTextEncoding(_self->ob_itself,
	                               inItem,
	                               &outScriptID);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outScriptID);
	return _res;
}

static PyObject *MenuObj_SetMenuItemHierarchicalID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	MenuID inHierID;
#ifndef SetMenuItemHierarchicalID
	PyMac_PRECHECK(SetMenuItemHierarchicalID);
#endif
	if (!PyArg_ParseTuple(_args, "hh",
	                      &inItem,
	                      &inHierID))
		return NULL;
	_err = SetMenuItemHierarchicalID(_self->ob_itself,
	                                 inItem,
	                                 inHierID);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuItemHierarchicalID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	MenuID outHierID;
#ifndef GetMenuItemHierarchicalID
	PyMac_PRECHECK(GetMenuItemHierarchicalID);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemHierarchicalID(_self->ob_itself,
	                                 inItem,
	                                 &outHierID);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("h",
	                     outHierID);
	return _res;
}

static PyObject *MenuObj_SetMenuItemFontID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	SInt16 inFontID;
#ifndef SetMenuItemFontID
	PyMac_PRECHECK(SetMenuItemFontID);
#endif
	if (!PyArg_ParseTuple(_args, "hh",
	                      &inItem,
	                      &inFontID))
		return NULL;
	_err = SetMenuItemFontID(_self->ob_itself,
	                         inItem,
	                         inFontID);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuItemFontID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	SInt16 outFontID;
#ifndef GetMenuItemFontID
	PyMac_PRECHECK(GetMenuItemFontID);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemFontID(_self->ob_itself,
	                         inItem,
	                         &outFontID);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("h",
	                     outFontID);
	return _res;
}

static PyObject *MenuObj_SetMenuItemRefCon(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	UInt32 inRefCon;
#ifndef SetMenuItemRefCon
	PyMac_PRECHECK(SetMenuItemRefCon);
#endif
	if (!PyArg_ParseTuple(_args, "hl",
	                      &inItem,
	                      &inRefCon))
		return NULL;
	_err = SetMenuItemRefCon(_self->ob_itself,
	                         inItem,
	                         inRefCon);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuItemRefCon(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	UInt32 outRefCon;
#ifndef GetMenuItemRefCon
	PyMac_PRECHECK(GetMenuItemRefCon);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemRefCon(_self->ob_itself,
	                         inItem,
	                         &outRefCon);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outRefCon);
	return _res;
}

#if !TARGET_API_MAC_CARBON

static PyObject *MenuObj_SetMenuItemRefCon2(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	UInt32 inRefCon2;
#ifndef SetMenuItemRefCon2
	PyMac_PRECHECK(SetMenuItemRefCon2);
#endif
	if (!PyArg_ParseTuple(_args, "hl",
	                      &inItem,
	                      &inRefCon2))
		return NULL;
	_err = SetMenuItemRefCon2(_self->ob_itself,
	                          inItem,
	                          inRefCon2);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if !TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuItemRefCon2(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	UInt32 outRefCon2;
#ifndef GetMenuItemRefCon2
	PyMac_PRECHECK(GetMenuItemRefCon2);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemRefCon2(_self->ob_itself,
	                          inItem,
	                          &outRefCon2);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outRefCon2);
	return _res;
}
#endif

static PyObject *MenuObj_SetMenuItemKeyGlyph(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	SInt16 inGlyph;
#ifndef SetMenuItemKeyGlyph
	PyMac_PRECHECK(SetMenuItemKeyGlyph);
#endif
	if (!PyArg_ParseTuple(_args, "hh",
	                      &inItem,
	                      &inGlyph))
		return NULL;
	_err = SetMenuItemKeyGlyph(_self->ob_itself,
	                           inItem,
	                           inGlyph);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_GetMenuItemKeyGlyph(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSErr _err;
	SInt16 inItem;
	SInt16 outGlyph;
#ifndef GetMenuItemKeyGlyph
	PyMac_PRECHECK(GetMenuItemKeyGlyph);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemKeyGlyph(_self->ob_itself,
	                           inItem,
	                           &outGlyph);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("h",
	                     outGlyph);
	return _res;
}

static PyObject *MenuObj_MacEnableMenuItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuItemIndex item;
#ifndef MacEnableMenuItem
	PyMac_PRECHECK(MacEnableMenuItem);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	MacEnableMenuItem(_self->ob_itself,
	                  item);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_DisableMenuItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuItemIndex item;
#ifndef DisableMenuItem
	PyMac_PRECHECK(DisableMenuItem);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	DisableMenuItem(_self->ob_itself,
	                item);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_IsMenuItemEnabled(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
	MenuItemIndex item;
#ifndef IsMenuItemEnabled
	PyMac_PRECHECK(IsMenuItemEnabled);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	_rv = IsMenuItemEnabled(_self->ob_itself,
	                        item);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}

static PyObject *MenuObj_EnableMenuItemIcon(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuItemIndex item;
#ifndef EnableMenuItemIcon
	PyMac_PRECHECK(EnableMenuItemIcon);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	EnableMenuItemIcon(_self->ob_itself,
	                   item);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_DisableMenuItemIcon(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuItemIndex item;
#ifndef DisableMenuItemIcon
	PyMac_PRECHECK(DisableMenuItemIcon);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	DisableMenuItemIcon(_self->ob_itself,
	                    item);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_IsMenuItemIconEnabled(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
	MenuItemIndex item;
#ifndef IsMenuItemIconEnabled
	PyMac_PRECHECK(IsMenuItemIconEnabled);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	_rv = IsMenuItemIconEnabled(_self->ob_itself,
	                            item);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_SetMenuItemHierarchicalMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inItem;
	MenuHandle inHierMenu;
#ifndef SetMenuItemHierarchicalMenu
	PyMac_PRECHECK(SetMenuItemHierarchicalMenu);
#endif
	if (!PyArg_ParseTuple(_args, "hO&",
	                      &inItem,
	                      MenuObj_Convert, &inHierMenu))
		return NULL;
	_err = SetMenuItemHierarchicalMenu(_self->ob_itself,
	                                   inItem,
	                                   inHierMenu);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuItemHierarchicalMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inItem;
	MenuHandle outHierMenu;
#ifndef GetMenuItemHierarchicalMenu
	PyMac_PRECHECK(GetMenuItemHierarchicalMenu);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemHierarchicalMenu(_self->ob_itself,
	                                   inItem,
	                                   &outHierMenu);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("O&",
	                     MenuObj_New, outHierMenu);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_CopyMenuItemTextAsCFString(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inItem;
	CFStringRef outString;
#ifndef CopyMenuItemTextAsCFString
	PyMac_PRECHECK(CopyMenuItemTextAsCFString);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = CopyMenuItemTextAsCFString(_self->ob_itself,
	                                  inItem,
	                                  &outString);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("O&",
	                     CFStringRefObj_New, outString);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_SetMenuItemTextWithCFString(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inItem;
	CFStringRef inString;
#ifndef SetMenuItemTextWithCFString
	PyMac_PRECHECK(SetMenuItemTextWithCFString);
#endif
	if (!PyArg_ParseTuple(_args, "hO&",
	                      &inItem,
	                      CFStringRefObj_Convert, &inString))
		return NULL;
	_err = SetMenuItemTextWithCFString(_self->ob_itself,
	                                   inItem,
	                                   inString);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuItemIndent(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inItem;
	UInt32 outIndent;
#ifndef GetMenuItemIndent
	PyMac_PRECHECK(GetMenuItemIndent);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &inItem))
		return NULL;
	_err = GetMenuItemIndent(_self->ob_itself,
	                         inItem,
	                         &outIndent);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outIndent);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_SetMenuItemIndent(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inItem;
	UInt32 inIndent;
#ifndef SetMenuItemIndent
	PyMac_PRECHECK(SetMenuItemIndent);
#endif
	if (!PyArg_ParseTuple(_args, "hl",
	                      &inItem,
	                      &inIndent))
		return NULL;
	_err = SetMenuItemIndent(_self->ob_itself,
	                         inItem,
	                         inIndent);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuItemCommandKey(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inItem;
	Boolean inGetVirtualKey;
	UInt16 outKey;
#ifndef GetMenuItemCommandKey
	PyMac_PRECHECK(GetMenuItemCommandKey);
#endif
	if (!PyArg_ParseTuple(_args, "hb",
	                      &inItem,
	                      &inGetVirtualKey))
		return NULL;
	_err = GetMenuItemCommandKey(_self->ob_itself,
	                             inItem,
	                             inGetVirtualKey,
	                             &outKey);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("H",
	                     outKey);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_SetMenuItemCommandKey(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex inItem;
	Boolean inSetVirtualKey;
	UInt16 inKey;
#ifndef SetMenuItemCommandKey
	PyMac_PRECHECK(SetMenuItemCommandKey);
#endif
	if (!PyArg_ParseTuple(_args, "hbH",
	                      &inItem,
	                      &inSetVirtualKey,
	                      &inKey))
		return NULL;
	_err = SetMenuItemCommandKey(_self->ob_itself,
	                             inItem,
	                             inSetVirtualKey,
	                             inKey);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuItemPropertyAttributes(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex item;
	OSType propertyCreator;
	OSType propertyTag;
	UInt32 attributes;
#ifndef GetMenuItemPropertyAttributes
	PyMac_PRECHECK(GetMenuItemPropertyAttributes);
#endif
	if (!PyArg_ParseTuple(_args, "hO&O&",
	                      &item,
	                      PyMac_GetOSType, &propertyCreator,
	                      PyMac_GetOSType, &propertyTag))
		return NULL;
	_err = GetMenuItemPropertyAttributes(_self->ob_itself,
	                                     item,
	                                     propertyCreator,
	                                     propertyTag,
	                                     &attributes);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     attributes);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_ChangeMenuItemPropertyAttributes(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex item;
	OSType propertyCreator;
	OSType propertyTag;
	UInt32 attributesToSet;
	UInt32 attributesToClear;
#ifndef ChangeMenuItemPropertyAttributes
	PyMac_PRECHECK(ChangeMenuItemPropertyAttributes);
#endif
	if (!PyArg_ParseTuple(_args, "hO&O&ll",
	                      &item,
	                      PyMac_GetOSType, &propertyCreator,
	                      PyMac_GetOSType, &propertyTag,
	                      &attributesToSet,
	                      &attributesToClear))
		return NULL;
	_err = ChangeMenuItemPropertyAttributes(_self->ob_itself,
	                                        item,
	                                        propertyCreator,
	                                        propertyTag,
	                                        attributesToSet,
	                                        attributesToClear);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuAttributes(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuAttributes outAttributes;
#ifndef GetMenuAttributes
	PyMac_PRECHECK(GetMenuAttributes);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = GetMenuAttributes(_self->ob_itself,
	                         &outAttributes);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outAttributes);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_ChangeMenuAttributes(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuAttributes setTheseAttributes;
	MenuAttributes clearTheseAttributes;
#ifndef ChangeMenuAttributes
	PyMac_PRECHECK(ChangeMenuAttributes);
#endif
	if (!PyArg_ParseTuple(_args, "ll",
	                      &setTheseAttributes,
	                      &clearTheseAttributes))
		return NULL;
	_err = ChangeMenuAttributes(_self->ob_itself,
	                            setTheseAttributes,
	                            clearTheseAttributes);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuItemAttributes(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex item;
	MenuItemAttributes outAttributes;
#ifndef GetMenuItemAttributes
	PyMac_PRECHECK(GetMenuItemAttributes);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	_err = GetMenuItemAttributes(_self->ob_itself,
	                             item,
	                             &outAttributes);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outAttributes);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_ChangeMenuItemAttributes(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex item;
	MenuItemAttributes setTheseAttributes;
	MenuItemAttributes clearTheseAttributes;
#ifndef ChangeMenuItemAttributes
	PyMac_PRECHECK(ChangeMenuItemAttributes);
#endif
	if (!PyArg_ParseTuple(_args, "hll",
	                      &item,
	                      &setTheseAttributes,
	                      &clearTheseAttributes))
		return NULL;
	_err = ChangeMenuItemAttributes(_self->ob_itself,
	                                item,
	                                setTheseAttributes,
	                                clearTheseAttributes);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_DisableAllMenuItems(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef DisableAllMenuItems
	PyMac_PRECHECK(DisableAllMenuItems);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	DisableAllMenuItems(_self->ob_itself);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_EnableAllMenuItems(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef EnableAllMenuItems
	PyMac_PRECHECK(EnableAllMenuItems);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	EnableAllMenuItems(_self->ob_itself);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_MenuHasEnabledItems(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
#ifndef MenuHasEnabledItems
	PyMac_PRECHECK(MenuHasEnabledItems);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = MenuHasEnabledItems(_self->ob_itself);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuType(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	UInt16 outType;
#ifndef GetMenuType
	PyMac_PRECHECK(GetMenuType);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = GetMenuType(_self->ob_itself,
	                   &outType);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("H",
	                     outType);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_CountMenuItemsWithCommandID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	ItemCount _rv;
	MenuCommand inCommandID;
#ifndef CountMenuItemsWithCommandID
	PyMac_PRECHECK(CountMenuItemsWithCommandID);
#endif
	if (!PyArg_ParseTuple(_args, "l",
	                      &inCommandID))
		return NULL;
	_rv = CountMenuItemsWithCommandID(_self->ob_itself,
	                                  inCommandID);
	_res = Py_BuildValue("l",
	                     _rv);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetIndMenuItemWithCommandID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuCommand inCommandID;
	UInt32 inItemIndex;
	MenuHandle outMenu;
	MenuItemIndex outIndex;
#ifndef GetIndMenuItemWithCommandID
	PyMac_PRECHECK(GetIndMenuItemWithCommandID);
#endif
	if (!PyArg_ParseTuple(_args, "ll",
	                      &inCommandID,
	                      &inItemIndex))
		return NULL;
	_err = GetIndMenuItemWithCommandID(_self->ob_itself,
	                                   inCommandID,
	                                   inItemIndex,
	                                   &outMenu,
	                                   &outIndex);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("O&h",
	                     MenuObj_New, outMenu,
	                     outIndex);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_EnableMenuCommand(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuCommand inCommandID;
#ifndef EnableMenuCommand
	PyMac_PRECHECK(EnableMenuCommand);
#endif
	if (!PyArg_ParseTuple(_args, "l",
	                      &inCommandID))
		return NULL;
	EnableMenuCommand(_self->ob_itself,
	                  inCommandID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_DisableMenuCommand(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuCommand inCommandID;
#ifndef DisableMenuCommand
	PyMac_PRECHECK(DisableMenuCommand);
#endif
	if (!PyArg_ParseTuple(_args, "l",
	                      &inCommandID))
		return NULL;
	DisableMenuCommand(_self->ob_itself,
	                   inCommandID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_IsMenuCommandEnabled(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
	MenuCommand inCommandID;
#ifndef IsMenuCommandEnabled
	PyMac_PRECHECK(IsMenuCommandEnabled);
#endif
	if (!PyArg_ParseTuple(_args, "l",
	                      &inCommandID))
		return NULL;
	_rv = IsMenuCommandEnabled(_self->ob_itself,
	                           inCommandID);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_SetMenuCommandMark(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuCommand inCommandID;
	UniChar inMark;
#ifndef SetMenuCommandMark
	PyMac_PRECHECK(SetMenuCommandMark);
#endif
	if (!PyArg_ParseTuple(_args, "lh",
	                      &inCommandID,
	                      &inMark))
		return NULL;
	_err = SetMenuCommandMark(_self->ob_itself,
	                          inCommandID,
	                          inMark);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuCommandMark(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuCommand inCommandID;
	UniChar outMark;
#ifndef GetMenuCommandMark
	PyMac_PRECHECK(GetMenuCommandMark);
#endif
	if (!PyArg_ParseTuple(_args, "l",
	                      &inCommandID))
		return NULL;
	_err = GetMenuCommandMark(_self->ob_itself,
	                          inCommandID,
	                          &outMark);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("h",
	                     outMark);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetMenuCommandPropertySize(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuCommand inCommandID;
	OSType inPropertyCreator;
	OSType inPropertyTag;
	ByteCount outSize;
#ifndef GetMenuCommandPropertySize
	PyMac_PRECHECK(GetMenuCommandPropertySize);
#endif
	if (!PyArg_ParseTuple(_args, "lO&O&",
	                      &inCommandID,
	                      PyMac_GetOSType, &inPropertyCreator,
	                      PyMac_GetOSType, &inPropertyTag))
		return NULL;
	_err = GetMenuCommandPropertySize(_self->ob_itself,
	                                  inCommandID,
	                                  inPropertyCreator,
	                                  inPropertyTag,
	                                  &outSize);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outSize);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_RemoveMenuCommandProperty(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuCommand inCommandID;
	OSType inPropertyCreator;
	OSType inPropertyTag;
#ifndef RemoveMenuCommandProperty
	PyMac_PRECHECK(RemoveMenuCommandProperty);
#endif
	if (!PyArg_ParseTuple(_args, "lO&O&",
	                      &inCommandID,
	                      PyMac_GetOSType, &inPropertyCreator,
	                      PyMac_GetOSType, &inPropertyTag))
		return NULL;
	_err = RemoveMenuCommandProperty(_self->ob_itself,
	                                 inCommandID,
	                                 inPropertyCreator,
	                                 inPropertyTag);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_IsMenuItemInvalid(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
	MenuItemIndex item;
#ifndef IsMenuItemInvalid
	PyMac_PRECHECK(IsMenuItemInvalid);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	_rv = IsMenuItemInvalid(_self->ob_itself,
	                        item);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_InvalidateMenuItems(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex firstItem;
	ItemCount numItems;
#ifndef InvalidateMenuItems
	PyMac_PRECHECK(InvalidateMenuItems);
#endif
	if (!PyArg_ParseTuple(_args, "hl",
	                      &firstItem,
	                      &numItems))
		return NULL;
	_err = InvalidateMenuItems(_self->ob_itself,
	                           firstItem,
	                           numItems);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_UpdateInvalidMenuItems(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
#ifndef UpdateInvalidMenuItems
	PyMac_PRECHECK(UpdateInvalidMenuItems);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = UpdateInvalidMenuItems(_self->ob_itself);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_CreateStandardFontMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex afterItem;
	MenuID firstHierMenuID;
	OptionBits options;
	ItemCount outHierMenuCount;
#ifndef CreateStandardFontMenu
	PyMac_PRECHECK(CreateStandardFontMenu);
#endif
	if (!PyArg_ParseTuple(_args, "hhl",
	                      &afterItem,
	                      &firstHierMenuID,
	                      &options))
		return NULL;
	_err = CreateStandardFontMenu(_self->ob_itself,
	                              afterItem,
	                              firstHierMenuID,
	                              options,
	                              &outHierMenuCount);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outHierMenuCount);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_UpdateStandardFontMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	ItemCount outHierMenuCount;
#ifndef UpdateStandardFontMenu
	PyMac_PRECHECK(UpdateStandardFontMenu);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = UpdateStandardFontMenu(_self->ob_itself,
	                              &outHierMenuCount);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("l",
	                     outHierMenuCount);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *MenuObj_GetFontFamilyFromMenuSelection(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuItemIndex item;
	FMFontFamily outFontFamily;
	FMFontStyle outStyle;
#ifndef GetFontFamilyFromMenuSelection
	PyMac_PRECHECK(GetFontFamilyFromMenuSelection);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &item))
		return NULL;
	_err = GetFontFamilyFromMenuSelection(_self->ob_itself,
	                                      item,
	                                      &outFontFamily,
	                                      &outStyle);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("hh",
	                     outFontFamily,
	                     outStyle);
	return _res;
}
#endif

static PyObject *MenuObj_GetMenuID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuID _rv;
#ifndef GetMenuID
	PyMac_PRECHECK(GetMenuID);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = GetMenuID(_self->ob_itself);
	_res = Py_BuildValue("h",
	                     _rv);
	return _res;
}

static PyObject *MenuObj_GetMenuWidth(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	SInt16 _rv;
#ifndef GetMenuWidth
	PyMac_PRECHECK(GetMenuWidth);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = GetMenuWidth(_self->ob_itself);
	_res = Py_BuildValue("h",
	                     _rv);
	return _res;
}

static PyObject *MenuObj_GetMenuHeight(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	SInt16 _rv;
#ifndef GetMenuHeight
	PyMac_PRECHECK(GetMenuHeight);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = GetMenuHeight(_self->ob_itself);
	_res = Py_BuildValue("h",
	                     _rv);
	return _res;
}

static PyObject *MenuObj_SetMenuID(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuID menuID;
#ifndef SetMenuID
	PyMac_PRECHECK(SetMenuID);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &menuID))
		return NULL;
	SetMenuID(_self->ob_itself,
	          menuID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_SetMenuWidth(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	SInt16 width;
#ifndef SetMenuWidth
	PyMac_PRECHECK(SetMenuWidth);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &width))
		return NULL;
	SetMenuWidth(_self->ob_itself,
	             width);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_SetMenuHeight(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	SInt16 height;
#ifndef SetMenuHeight
	PyMac_PRECHECK(SetMenuHeight);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &height))
		return NULL;
	SetMenuHeight(_self->ob_itself,
	              height);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_as_Resource(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Handle _rv;
#ifndef as_Resource
	PyMac_PRECHECK(as_Resource);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = as_Resource(_self->ob_itself);
	_res = Py_BuildValue("O&",
	                     ResObj_New, _rv);
	return _res;
}

static PyObject *MenuObj_AppendMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Str255 data;
#ifndef AppendMenu
	PyMac_PRECHECK(AppendMenu);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      PyMac_GetStr255, data))
		return NULL;
	AppendMenu(_self->ob_itself,
	           data);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_InsertMenu(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short beforeID;
#ifndef InsertMenu
	PyMac_PRECHECK(InsertMenu);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &beforeID))
		return NULL;
	InsertMenu(_self->ob_itself,
	           beforeID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_InsertMenuItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Str255 itemString;
	short afterItem;
#ifndef InsertMenuItem
	PyMac_PRECHECK(InsertMenuItem);
#endif
	if (!PyArg_ParseTuple(_args, "O&h",
	                      PyMac_GetStr255, itemString,
	                      &afterItem))
		return NULL;
	InsertMenuItem(_self->ob_itself,
	               itemString,
	               afterItem);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_EnableMenuItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	UInt16 item;
#ifndef EnableMenuItem
	PyMac_PRECHECK(EnableMenuItem);
#endif
	if (!PyArg_ParseTuple(_args, "H",
	                      &item))
		return NULL;
	EnableMenuItem(_self->ob_itself,
	               item);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *MenuObj_CheckMenuItem(MenuObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short item;
	Boolean checked;
#ifndef CheckMenuItem
	PyMac_PRECHECK(CheckMenuItem);
#endif
	if (!PyArg_ParseTuple(_args, "hb",
	                      &item,
	                      &checked))
		return NULL;
	CheckMenuItem(_self->ob_itself,
	              item,
	              checked);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyMethodDef MenuObj_methods[] = {
	{"DisposeMenu", (PyCFunction)MenuObj_DisposeMenu, 1,
	 "() -> None"},
	{"CalcMenuSize", (PyCFunction)MenuObj_CalcMenuSize, 1,
	 "() -> None"},

#if !TARGET_API_MAC_CARBON
	{"CountMItems", (PyCFunction)MenuObj_CountMItems, 1,
	 "() -> (short _rv)"},
#endif
	{"CountMenuItems", (PyCFunction)MenuObj_CountMenuItems, 1,
	 "() -> (short _rv)"},
	{"GetMenuFont", (PyCFunction)MenuObj_GetMenuFont, 1,
	 "() -> (SInt16 outFontID, UInt16 outFontSize)"},
	{"SetMenuFont", (PyCFunction)MenuObj_SetMenuFont, 1,
	 "(SInt16 inFontID, UInt16 inFontSize) -> None"},
	{"GetMenuExcludesMarkColumn", (PyCFunction)MenuObj_GetMenuExcludesMarkColumn, 1,
	 "() -> (Boolean _rv)"},
	{"SetMenuExcludesMarkColumn", (PyCFunction)MenuObj_SetMenuExcludesMarkColumn, 1,
	 "(Boolean excludesMark) -> None"},

#if TARGET_API_MAC_CARBON
	{"IsValidMenu", (PyCFunction)MenuObj_IsValidMenu, 1,
	 "() -> (Boolean _rv)"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuRetainCount", (PyCFunction)MenuObj_GetMenuRetainCount, 1,
	 "() -> (ItemCount _rv)"},
#endif

#if TARGET_API_MAC_CARBON
	{"RetainMenu", (PyCFunction)MenuObj_RetainMenu, 1,
	 "() -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"ReleaseMenu", (PyCFunction)MenuObj_ReleaseMenu, 1,
	 "() -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"DuplicateMenu", (PyCFunction)MenuObj_DuplicateMenu, 1,
	 "() -> (MenuHandle outMenu)"},
#endif

#if TARGET_API_MAC_CARBON
	{"CopyMenuTitleAsCFString", (PyCFunction)MenuObj_CopyMenuTitleAsCFString, 1,
	 "() -> (CFStringRef outString)"},
#endif

#if TARGET_API_MAC_CARBON
	{"SetMenuTitleWithCFString", (PyCFunction)MenuObj_SetMenuTitleWithCFString, 1,
	 "(CFStringRef inString) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"InvalidateMenuSize", (PyCFunction)MenuObj_InvalidateMenuSize, 1,
	 "() -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"IsMenuSizeInvalid", (PyCFunction)MenuObj_IsMenuSizeInvalid, 1,
	 "() -> (Boolean _rv)"},
#endif
	{"MacAppendMenu", (PyCFunction)MenuObj_MacAppendMenu, 1,
	 "(Str255 data) -> None"},
	{"InsertResMenu", (PyCFunction)MenuObj_InsertResMenu, 1,
	 "(ResType theType, short afterItem) -> None"},
	{"AppendResMenu", (PyCFunction)MenuObj_AppendResMenu, 1,
	 "(ResType theType) -> None"},
	{"MacInsertMenuItem", (PyCFunction)MenuObj_MacInsertMenuItem, 1,
	 "(Str255 itemString, short afterItem) -> None"},
	{"DeleteMenuItem", (PyCFunction)MenuObj_DeleteMenuItem, 1,
	 "(short item) -> None"},
	{"InsertFontResMenu", (PyCFunction)MenuObj_InsertFontResMenu, 1,
	 "(short afterItem, short scriptFilter) -> None"},
	{"InsertIntlResMenu", (PyCFunction)MenuObj_InsertIntlResMenu, 1,
	 "(ResType theType, short afterItem, short scriptFilter) -> None"},
	{"AppendMenuItemText", (PyCFunction)MenuObj_AppendMenuItemText, 1,
	 "(Str255 inString) -> None"},
	{"InsertMenuItemText", (PyCFunction)MenuObj_InsertMenuItemText, 1,
	 "(Str255 inString, MenuItemIndex afterItem) -> None"},

#if TARGET_API_MAC_CARBON
	{"CopyMenuItems", (PyCFunction)MenuObj_CopyMenuItems, 1,
	 "(MenuItemIndex inFirstItem, ItemCount inNumItems, MenuHandle inDestMenu, MenuItemIndex inInsertAfter) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"DeleteMenuItems", (PyCFunction)MenuObj_DeleteMenuItems, 1,
	 "(MenuItemIndex inFirstItem, ItemCount inNumItems) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"AppendMenuItemTextWithCFString", (PyCFunction)MenuObj_AppendMenuItemTextWithCFString, 1,
	 "(CFStringRef inString, MenuItemAttributes inAttributes, MenuCommand inCommandID) -> (MenuItemIndex outNewItem)"},
#endif

#if TARGET_API_MAC_CARBON
	{"InsertMenuItemTextWithCFString", (PyCFunction)MenuObj_InsertMenuItemTextWithCFString, 1,
	 "(CFStringRef inString, MenuItemIndex inAfterItem, MenuItemAttributes inAttributes, MenuCommand inCommandID) -> None"},
#endif
	{"PopUpMenuSelect", (PyCFunction)MenuObj_PopUpMenuSelect, 1,
	 "(short top, short left, short popUpItem) -> (long _rv)"},

#if TARGET_API_MAC_CARBON
	{"InvalidateMenuEnabling", (PyCFunction)MenuObj_InvalidateMenuEnabling, 1,
	 "() -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"IsMenuBarInvalid", (PyCFunction)MenuObj_IsMenuBarInvalid, 1,
	 "() -> (Boolean _rv)"},
#endif
	{"MacInsertMenu", (PyCFunction)MenuObj_MacInsertMenu, 1,
	 "(MenuID beforeID) -> None"},

#if TARGET_API_MAC_CARBON
	{"SetRootMenu", (PyCFunction)MenuObj_SetRootMenu, 1,
	 "() -> None"},
#endif

#if !TARGET_API_MAC_CARBON
	{"CheckItem", (PyCFunction)MenuObj_CheckItem, 1,
	 "(short item, Boolean checked) -> None"},
#endif
	{"MacCheckMenuItem", (PyCFunction)MenuObj_MacCheckMenuItem, 1,
	 "(short item, Boolean checked) -> None"},
	{"SetMenuItemText", (PyCFunction)MenuObj_SetMenuItemText, 1,
	 "(short item, Str255 itemString) -> None"},
	{"GetMenuItemText", (PyCFunction)MenuObj_GetMenuItemText, 1,
	 "(short item) -> (Str255 itemString)"},
	{"SetItemMark", (PyCFunction)MenuObj_SetItemMark, 1,
	 "(short item, CharParameter markChar) -> None"},
	{"GetItemMark", (PyCFunction)MenuObj_GetItemMark, 1,
	 "(short item) -> (CharParameter markChar)"},
	{"SetItemCmd", (PyCFunction)MenuObj_SetItemCmd, 1,
	 "(short item, CharParameter cmdChar) -> None"},
	{"GetItemCmd", (PyCFunction)MenuObj_GetItemCmd, 1,
	 "(short item) -> (CharParameter cmdChar)"},
	{"SetItemIcon", (PyCFunction)MenuObj_SetItemIcon, 1,
	 "(short item, short iconIndex) -> None"},
	{"GetItemIcon", (PyCFunction)MenuObj_GetItemIcon, 1,
	 "(short item) -> (short iconIndex)"},
	{"SetItemStyle", (PyCFunction)MenuObj_SetItemStyle, 1,
	 "(short item, StyleParameter chStyle) -> None"},
	{"GetItemStyle", (PyCFunction)MenuObj_GetItemStyle, 1,
	 "(short item) -> (Style chStyle)"},

#if !TARGET_API_MAC_CARBON
	{"DisableItem", (PyCFunction)MenuObj_DisableItem, 1,
	 "(short item) -> None"},
#endif

#if !TARGET_API_MAC_CARBON
	{"EnableItem", (PyCFunction)MenuObj_EnableItem, 1,
	 "(short item) -> None"},
#endif
	{"SetMenuItemCommandID", (PyCFunction)MenuObj_SetMenuItemCommandID, 1,
	 "(SInt16 inItem, MenuCommand inCommandID) -> None"},
	{"GetMenuItemCommandID", (PyCFunction)MenuObj_GetMenuItemCommandID, 1,
	 "(SInt16 inItem) -> (MenuCommand outCommandID)"},
	{"SetMenuItemModifiers", (PyCFunction)MenuObj_SetMenuItemModifiers, 1,
	 "(SInt16 inItem, UInt8 inModifiers) -> None"},
	{"GetMenuItemModifiers", (PyCFunction)MenuObj_GetMenuItemModifiers, 1,
	 "(SInt16 inItem) -> (UInt8 outModifiers)"},
	{"SetMenuItemIconHandle", (PyCFunction)MenuObj_SetMenuItemIconHandle, 1,
	 "(SInt16 inItem, UInt8 inIconType, Handle inIconHandle) -> None"},
	{"GetMenuItemIconHandle", (PyCFunction)MenuObj_GetMenuItemIconHandle, 1,
	 "(SInt16 inItem) -> (UInt8 outIconType, Handle outIconHandle)"},
	{"SetMenuItemTextEncoding", (PyCFunction)MenuObj_SetMenuItemTextEncoding, 1,
	 "(SInt16 inItem, TextEncoding inScriptID) -> None"},
	{"GetMenuItemTextEncoding", (PyCFunction)MenuObj_GetMenuItemTextEncoding, 1,
	 "(SInt16 inItem) -> (TextEncoding outScriptID)"},
	{"SetMenuItemHierarchicalID", (PyCFunction)MenuObj_SetMenuItemHierarchicalID, 1,
	 "(SInt16 inItem, MenuID inHierID) -> None"},
	{"GetMenuItemHierarchicalID", (PyCFunction)MenuObj_GetMenuItemHierarchicalID, 1,
	 "(SInt16 inItem) -> (MenuID outHierID)"},
	{"SetMenuItemFontID", (PyCFunction)MenuObj_SetMenuItemFontID, 1,
	 "(SInt16 inItem, SInt16 inFontID) -> None"},
	{"GetMenuItemFontID", (PyCFunction)MenuObj_GetMenuItemFontID, 1,
	 "(SInt16 inItem) -> (SInt16 outFontID)"},
	{"SetMenuItemRefCon", (PyCFunction)MenuObj_SetMenuItemRefCon, 1,
	 "(SInt16 inItem, UInt32 inRefCon) -> None"},
	{"GetMenuItemRefCon", (PyCFunction)MenuObj_GetMenuItemRefCon, 1,
	 "(SInt16 inItem) -> (UInt32 outRefCon)"},

#if !TARGET_API_MAC_CARBON
	{"SetMenuItemRefCon2", (PyCFunction)MenuObj_SetMenuItemRefCon2, 1,
	 "(SInt16 inItem, UInt32 inRefCon2) -> None"},
#endif

#if !TARGET_API_MAC_CARBON
	{"GetMenuItemRefCon2", (PyCFunction)MenuObj_GetMenuItemRefCon2, 1,
	 "(SInt16 inItem) -> (UInt32 outRefCon2)"},
#endif
	{"SetMenuItemKeyGlyph", (PyCFunction)MenuObj_SetMenuItemKeyGlyph, 1,
	 "(SInt16 inItem, SInt16 inGlyph) -> None"},
	{"GetMenuItemKeyGlyph", (PyCFunction)MenuObj_GetMenuItemKeyGlyph, 1,
	 "(SInt16 inItem) -> (SInt16 outGlyph)"},
	{"MacEnableMenuItem", (PyCFunction)MenuObj_MacEnableMenuItem, 1,
	 "(MenuItemIndex item) -> None"},
	{"DisableMenuItem", (PyCFunction)MenuObj_DisableMenuItem, 1,
	 "(MenuItemIndex item) -> None"},
	{"IsMenuItemEnabled", (PyCFunction)MenuObj_IsMenuItemEnabled, 1,
	 "(MenuItemIndex item) -> (Boolean _rv)"},
	{"EnableMenuItemIcon", (PyCFunction)MenuObj_EnableMenuItemIcon, 1,
	 "(MenuItemIndex item) -> None"},
	{"DisableMenuItemIcon", (PyCFunction)MenuObj_DisableMenuItemIcon, 1,
	 "(MenuItemIndex item) -> None"},
	{"IsMenuItemIconEnabled", (PyCFunction)MenuObj_IsMenuItemIconEnabled, 1,
	 "(MenuItemIndex item) -> (Boolean _rv)"},

#if TARGET_API_MAC_CARBON
	{"SetMenuItemHierarchicalMenu", (PyCFunction)MenuObj_SetMenuItemHierarchicalMenu, 1,
	 "(MenuItemIndex inItem, MenuHandle inHierMenu) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuItemHierarchicalMenu", (PyCFunction)MenuObj_GetMenuItemHierarchicalMenu, 1,
	 "(MenuItemIndex inItem) -> (MenuHandle outHierMenu)"},
#endif

#if TARGET_API_MAC_CARBON
	{"CopyMenuItemTextAsCFString", (PyCFunction)MenuObj_CopyMenuItemTextAsCFString, 1,
	 "(MenuItemIndex inItem) -> (CFStringRef outString)"},
#endif

#if TARGET_API_MAC_CARBON
	{"SetMenuItemTextWithCFString", (PyCFunction)MenuObj_SetMenuItemTextWithCFString, 1,
	 "(MenuItemIndex inItem, CFStringRef inString) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuItemIndent", (PyCFunction)MenuObj_GetMenuItemIndent, 1,
	 "(MenuItemIndex inItem) -> (UInt32 outIndent)"},
#endif

#if TARGET_API_MAC_CARBON
	{"SetMenuItemIndent", (PyCFunction)MenuObj_SetMenuItemIndent, 1,
	 "(MenuItemIndex inItem, UInt32 inIndent) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuItemCommandKey", (PyCFunction)MenuObj_GetMenuItemCommandKey, 1,
	 "(MenuItemIndex inItem, Boolean inGetVirtualKey) -> (UInt16 outKey)"},
#endif

#if TARGET_API_MAC_CARBON
	{"SetMenuItemCommandKey", (PyCFunction)MenuObj_SetMenuItemCommandKey, 1,
	 "(MenuItemIndex inItem, Boolean inSetVirtualKey, UInt16 inKey) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuItemPropertyAttributes", (PyCFunction)MenuObj_GetMenuItemPropertyAttributes, 1,
	 "(MenuItemIndex item, OSType propertyCreator, OSType propertyTag) -> (UInt32 attributes)"},
#endif

#if TARGET_API_MAC_CARBON
	{"ChangeMenuItemPropertyAttributes", (PyCFunction)MenuObj_ChangeMenuItemPropertyAttributes, 1,
	 "(MenuItemIndex item, OSType propertyCreator, OSType propertyTag, UInt32 attributesToSet, UInt32 attributesToClear) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuAttributes", (PyCFunction)MenuObj_GetMenuAttributes, 1,
	 "() -> (MenuAttributes outAttributes)"},
#endif

#if TARGET_API_MAC_CARBON
	{"ChangeMenuAttributes", (PyCFunction)MenuObj_ChangeMenuAttributes, 1,
	 "(MenuAttributes setTheseAttributes, MenuAttributes clearTheseAttributes) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuItemAttributes", (PyCFunction)MenuObj_GetMenuItemAttributes, 1,
	 "(MenuItemIndex item) -> (MenuItemAttributes outAttributes)"},
#endif

#if TARGET_API_MAC_CARBON
	{"ChangeMenuItemAttributes", (PyCFunction)MenuObj_ChangeMenuItemAttributes, 1,
	 "(MenuItemIndex item, MenuItemAttributes setTheseAttributes, MenuItemAttributes clearTheseAttributes) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"DisableAllMenuItems", (PyCFunction)MenuObj_DisableAllMenuItems, 1,
	 "() -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"EnableAllMenuItems", (PyCFunction)MenuObj_EnableAllMenuItems, 1,
	 "() -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"MenuHasEnabledItems", (PyCFunction)MenuObj_MenuHasEnabledItems, 1,
	 "() -> (Boolean _rv)"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuType", (PyCFunction)MenuObj_GetMenuType, 1,
	 "() -> (UInt16 outType)"},
#endif

#if TARGET_API_MAC_CARBON
	{"CountMenuItemsWithCommandID", (PyCFunction)MenuObj_CountMenuItemsWithCommandID, 1,
	 "(MenuCommand inCommandID) -> (ItemCount _rv)"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetIndMenuItemWithCommandID", (PyCFunction)MenuObj_GetIndMenuItemWithCommandID, 1,
	 "(MenuCommand inCommandID, UInt32 inItemIndex) -> (MenuHandle outMenu, MenuItemIndex outIndex)"},
#endif

#if TARGET_API_MAC_CARBON
	{"EnableMenuCommand", (PyCFunction)MenuObj_EnableMenuCommand, 1,
	 "(MenuCommand inCommandID) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"DisableMenuCommand", (PyCFunction)MenuObj_DisableMenuCommand, 1,
	 "(MenuCommand inCommandID) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"IsMenuCommandEnabled", (PyCFunction)MenuObj_IsMenuCommandEnabled, 1,
	 "(MenuCommand inCommandID) -> (Boolean _rv)"},
#endif

#if TARGET_API_MAC_CARBON
	{"SetMenuCommandMark", (PyCFunction)MenuObj_SetMenuCommandMark, 1,
	 "(MenuCommand inCommandID, UniChar inMark) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuCommandMark", (PyCFunction)MenuObj_GetMenuCommandMark, 1,
	 "(MenuCommand inCommandID) -> (UniChar outMark)"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetMenuCommandPropertySize", (PyCFunction)MenuObj_GetMenuCommandPropertySize, 1,
	 "(MenuCommand inCommandID, OSType inPropertyCreator, OSType inPropertyTag) -> (ByteCount outSize)"},
#endif

#if TARGET_API_MAC_CARBON
	{"RemoveMenuCommandProperty", (PyCFunction)MenuObj_RemoveMenuCommandProperty, 1,
	 "(MenuCommand inCommandID, OSType inPropertyCreator, OSType inPropertyTag) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"IsMenuItemInvalid", (PyCFunction)MenuObj_IsMenuItemInvalid, 1,
	 "(MenuItemIndex item) -> (Boolean _rv)"},
#endif

#if TARGET_API_MAC_CARBON
	{"InvalidateMenuItems", (PyCFunction)MenuObj_InvalidateMenuItems, 1,
	 "(MenuItemIndex firstItem, ItemCount numItems) -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"UpdateInvalidMenuItems", (PyCFunction)MenuObj_UpdateInvalidMenuItems, 1,
	 "() -> None"},
#endif

#if TARGET_API_MAC_CARBON
	{"CreateStandardFontMenu", (PyCFunction)MenuObj_CreateStandardFontMenu, 1,
	 "(MenuItemIndex afterItem, MenuID firstHierMenuID, OptionBits options) -> (ItemCount outHierMenuCount)"},
#endif

#if TARGET_API_MAC_CARBON
	{"UpdateStandardFontMenu", (PyCFunction)MenuObj_UpdateStandardFontMenu, 1,
	 "() -> (ItemCount outHierMenuCount)"},
#endif

#if TARGET_API_MAC_CARBON
	{"GetFontFamilyFromMenuSelection", (PyCFunction)MenuObj_GetFontFamilyFromMenuSelection, 1,
	 "(MenuItemIndex item) -> (FMFontFamily outFontFamily, FMFontStyle outStyle)"},
#endif
	{"GetMenuID", (PyCFunction)MenuObj_GetMenuID, 1,
	 "() -> (MenuID _rv)"},
	{"GetMenuWidth", (PyCFunction)MenuObj_GetMenuWidth, 1,
	 "() -> (SInt16 _rv)"},
	{"GetMenuHeight", (PyCFunction)MenuObj_GetMenuHeight, 1,
	 "() -> (SInt16 _rv)"},
	{"SetMenuID", (PyCFunction)MenuObj_SetMenuID, 1,
	 "(MenuID menuID) -> None"},
	{"SetMenuWidth", (PyCFunction)MenuObj_SetMenuWidth, 1,
	 "(SInt16 width) -> None"},
	{"SetMenuHeight", (PyCFunction)MenuObj_SetMenuHeight, 1,
	 "(SInt16 height) -> None"},
	{"as_Resource", (PyCFunction)MenuObj_as_Resource, 1,
	 "() -> (Handle _rv)"},
	{"AppendMenu", (PyCFunction)MenuObj_AppendMenu, 1,
	 "(Str255 data) -> None"},
	{"InsertMenu", (PyCFunction)MenuObj_InsertMenu, 1,
	 "(short beforeID) -> None"},
	{"InsertMenuItem", (PyCFunction)MenuObj_InsertMenuItem, 1,
	 "(Str255 itemString, short afterItem) -> None"},
	{"EnableMenuItem", (PyCFunction)MenuObj_EnableMenuItem, 1,
	 "(UInt16 item) -> None"},
	{"CheckMenuItem", (PyCFunction)MenuObj_CheckMenuItem, 1,
	 "(short item, Boolean checked) -> None"},
	{NULL, NULL, 0}
};

PyMethodChain MenuObj_chain = { MenuObj_methods, NULL };

static PyObject *MenuObj_getattr(MenuObject *self, char *name)
{
	return Py_FindMethodInChain(&MenuObj_chain, (PyObject *)self, name);
}

#define MenuObj_setattr NULL

#define MenuObj_compare NULL

#define MenuObj_repr NULL

#define MenuObj_hash NULL

PyTypeObject Menu_Type = {
	PyObject_HEAD_INIT(NULL)
	0, /*ob_size*/
	"_Menu.Menu", /*tp_name*/
	sizeof(MenuObject), /*tp_basicsize*/
	0, /*tp_itemsize*/
	/* methods */
	(destructor) MenuObj_dealloc, /*tp_dealloc*/
	0, /*tp_print*/
	(getattrfunc) MenuObj_getattr, /*tp_getattr*/
	(setattrfunc) MenuObj_setattr, /*tp_setattr*/
	(cmpfunc) MenuObj_compare, /*tp_compare*/
	(reprfunc) MenuObj_repr, /*tp_repr*/
	(PyNumberMethods *)0, /* tp_as_number */
	(PySequenceMethods *)0, /* tp_as_sequence */
	(PyMappingMethods *)0, /* tp_as_mapping */
	(hashfunc) MenuObj_hash, /*tp_hash*/
};

/* ---------------------- End object type Menu ---------------------- */


#if !TARGET_API_MAC_CARBON

static PyObject *Menu_InitProcMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short resID;
#ifndef InitProcMenu
	PyMac_PRECHECK(InitProcMenu);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &resID))
		return NULL;
	InitProcMenu(resID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

#if !TARGET_API_MAC_CARBON

static PyObject *Menu_InitMenus(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef InitMenus
	PyMac_PRECHECK(InitMenus);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	InitMenus();
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

static PyObject *Menu_NewMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuHandle _rv;
	MenuID menuID;
	Str255 menuTitle;
#ifndef NewMenu
	PyMac_PRECHECK(NewMenu);
#endif
	if (!PyArg_ParseTuple(_args, "hO&",
	                      &menuID,
	                      PyMac_GetStr255, menuTitle))
		return NULL;
	_rv = NewMenu(menuID,
	              menuTitle);
	_res = Py_BuildValue("O&",
	                     MenuObj_New, _rv);
	return _res;
}

static PyObject *Menu_MacGetMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuHandle _rv;
	short resourceID;
#ifndef MacGetMenu
	PyMac_PRECHECK(MacGetMenu);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &resourceID))
		return NULL;
	_rv = MacGetMenu(resourceID);
	_res = Py_BuildValue("O&",
	                     MenuObj_New, _rv);
	return _res;
}

#if TARGET_API_MAC_CARBON

static PyObject *Menu_CreateNewMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuID inMenuID;
	MenuAttributes inMenuAttributes;
	MenuHandle outMenuRef;
#ifndef CreateNewMenu
	PyMac_PRECHECK(CreateNewMenu);
#endif
	if (!PyArg_ParseTuple(_args, "hl",
	                      &inMenuID,
	                      &inMenuAttributes))
		return NULL;
	_err = CreateNewMenu(inMenuID,
	                     inMenuAttributes,
	                     &outMenuRef);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("O&",
	                     MenuObj_New, outMenuRef);
	return _res;
}
#endif

static PyObject *Menu_MenuKey(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	long _rv;
	CharParameter ch;
#ifndef MenuKey
	PyMac_PRECHECK(MenuKey);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &ch))
		return NULL;
	_rv = MenuKey(ch);
	_res = Py_BuildValue("l",
	                     _rv);
	return _res;
}

static PyObject *Menu_MenuSelect(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	long _rv;
	Point startPt;
#ifndef MenuSelect
	PyMac_PRECHECK(MenuSelect);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      PyMac_GetPoint, &startPt))
		return NULL;
	_rv = MenuSelect(startPt);
	_res = Py_BuildValue("l",
	                     _rv);
	return _res;
}

static PyObject *Menu_MenuChoice(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	long _rv;
#ifndef MenuChoice
	PyMac_PRECHECK(MenuChoice);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = MenuChoice();
	_res = Py_BuildValue("l",
	                     _rv);
	return _res;
}

static PyObject *Menu_MenuEvent(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	UInt32 _rv;
	EventRecord inEvent;
#ifndef MenuEvent
	PyMac_PRECHECK(MenuEvent);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      PyMac_GetEventRecord, &inEvent))
		return NULL;
	_rv = MenuEvent(&inEvent);
	_res = Py_BuildValue("l",
	                     _rv);
	return _res;
}

static PyObject *Menu_GetMBarHeight(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short _rv;
#ifndef GetMBarHeight
	PyMac_PRECHECK(GetMBarHeight);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = GetMBarHeight();
	_res = Py_BuildValue("h",
	                     _rv);
	return _res;
}

static PyObject *Menu_MacDrawMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef MacDrawMenuBar
	PyMac_PRECHECK(MacDrawMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	MacDrawMenuBar();
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *Menu_InvalMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef InvalMenuBar
	PyMac_PRECHECK(InvalMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	InvalMenuBar();
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *Menu_HiliteMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuID menuID;
#ifndef HiliteMenu
	PyMac_PRECHECK(HiliteMenu);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &menuID))
		return NULL;
	HiliteMenu(menuID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *Menu_GetNewMBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuBarHandle _rv;
	short menuBarID;
#ifndef GetNewMBar
	PyMac_PRECHECK(GetNewMBar);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &menuBarID))
		return NULL;
	_rv = GetNewMBar(menuBarID);
	_res = Py_BuildValue("O&",
	                     ResObj_New, _rv);
	return _res;
}

static PyObject *Menu_GetMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuBarHandle _rv;
#ifndef GetMenuBar
	PyMac_PRECHECK(GetMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = GetMenuBar();
	_res = Py_BuildValue("O&",
	                     ResObj_New, _rv);
	return _res;
}

static PyObject *Menu_SetMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuBarHandle mbar;
#ifndef SetMenuBar
	PyMac_PRECHECK(SetMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      ResObj_Convert, &mbar))
		return NULL;
	SetMenuBar(mbar);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

#if TARGET_API_MAC_CARBON

static PyObject *Menu_DuplicateMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuBarHandle inMbar;
	MenuBarHandle outMbar;
#ifndef DuplicateMenuBar
	PyMac_PRECHECK(DuplicateMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      ResObj_Convert, &inMbar))
		return NULL;
	_err = DuplicateMenuBar(inMbar,
	                        &outMbar);
	if (_err != noErr) return PyMac_Error(_err);
	_res = Py_BuildValue("O&",
	                     ResObj_New, outMbar);
	return _res;
}
#endif

#if TARGET_API_MAC_CARBON

static PyObject *Menu_DisposeMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
	MenuBarHandle inMbar;
#ifndef DisposeMenuBar
	PyMac_PRECHECK(DisposeMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      ResObj_Convert, &inMbar))
		return NULL;
	_err = DisposeMenuBar(inMbar);
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

static PyObject *Menu_GetMenuHandle(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuHandle _rv;
	MenuID menuID;
#ifndef GetMenuHandle
	PyMac_PRECHECK(GetMenuHandle);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &menuID))
		return NULL;
	_rv = GetMenuHandle(menuID);
	_res = Py_BuildValue("O&",
	                     MenuObj_New, _rv);
	return _res;
}

static PyObject *Menu_MacDeleteMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuID menuID;
#ifndef MacDeleteMenu
	PyMac_PRECHECK(MacDeleteMenu);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &menuID))
		return NULL;
	MacDeleteMenu(menuID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *Menu_ClearMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef ClearMenuBar
	PyMac_PRECHECK(ClearMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	ClearMenuBar();
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

#if !TARGET_API_MAC_CARBON

static PyObject *Menu_SetMenuFlash(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short count;
#ifndef SetMenuFlash
	PyMac_PRECHECK(SetMenuFlash);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &count))
		return NULL;
	SetMenuFlash(count);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

static PyObject *Menu_SetMenuFlashCount(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short count;
#ifndef SetMenuFlashCount
	PyMac_PRECHECK(SetMenuFlashCount);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &count))
		return NULL;
	SetMenuFlashCount(count);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *Menu_FlashMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuID menuID;
#ifndef FlashMenuBar
	PyMac_PRECHECK(FlashMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &menuID))
		return NULL;
	FlashMenuBar(menuID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

#if !TARGET_API_MAC_CARBON

static PyObject *Menu_SystemEdit(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
	short editCmd;
#ifndef SystemEdit
	PyMac_PRECHECK(SystemEdit);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &editCmd))
		return NULL;
	_rv = SystemEdit(editCmd);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}
#endif

#if !TARGET_API_MAC_CARBON

static PyObject *Menu_SystemMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	long menuResult;
#ifndef SystemMenu
	PyMac_PRECHECK(SystemMenu);
#endif
	if (!PyArg_ParseTuple(_args, "l",
	                      &menuResult))
		return NULL;
	SystemMenu(menuResult);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

static PyObject *Menu_IsMenuBarVisible(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
#ifndef IsMenuBarVisible
	PyMac_PRECHECK(IsMenuBarVisible);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = IsMenuBarVisible();
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}

static PyObject *Menu_ShowMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef ShowMenuBar
	PyMac_PRECHECK(ShowMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	ShowMenuBar();
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *Menu_HideMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef HideMenuBar
	PyMac_PRECHECK(HideMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	HideMenuBar();
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

#if TARGET_API_MAC_CARBON

static PyObject *Menu_AcquireRootMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuHandle _rv;
#ifndef AcquireRootMenu
	PyMac_PRECHECK(AcquireRootMenu);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = AcquireRootMenu();
	_res = Py_BuildValue("O&",
	                     MenuObj_New, _rv);
	return _res;
}
#endif

static PyObject *Menu_DeleteMCEntries(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuID menuID;
	short menuItem;
#ifndef DeleteMCEntries
	PyMac_PRECHECK(DeleteMCEntries);
#endif
	if (!PyArg_ParseTuple(_args, "hh",
	                      &menuID,
	                      &menuItem))
		return NULL;
	DeleteMCEntries(menuID,
	                menuItem);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *Menu_InitContextualMenus(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	OSStatus _err;
#ifndef InitContextualMenus
	PyMac_PRECHECK(InitContextualMenus);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_err = InitContextualMenus();
	if (_err != noErr) return PyMac_Error(_err);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *Menu_IsShowContextualMenuClick(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Boolean _rv;
	EventRecord inEvent;
#ifndef IsShowContextualMenuClick
	PyMac_PRECHECK(IsShowContextualMenuClick);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      PyMac_GetEventRecord, &inEvent))
		return NULL;
	_rv = IsShowContextualMenuClick(&inEvent);
	_res = Py_BuildValue("b",
	                     _rv);
	return _res;
}

static PyObject *Menu_LMGetTheMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	SInt16 _rv;
#ifndef LMGetTheMenu
	PyMac_PRECHECK(LMGetTheMenu);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	_rv = LMGetTheMenu();
	_res = Py_BuildValue("h",
	                     _rv);
	return _res;
}

#if !TARGET_API_MAC_CARBON

static PyObject *Menu_OpenDeskAcc(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	Str255 name;
#ifndef OpenDeskAcc
	PyMac_PRECHECK(OpenDeskAcc);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      PyMac_GetStr255, name))
		return NULL;
	OpenDeskAcc(name);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}
#endif

static PyObject *Menu_as_Menu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuHandle _rv;
	Handle h;
#ifndef as_Menu
	PyMac_PRECHECK(as_Menu);
#endif
	if (!PyArg_ParseTuple(_args, "O&",
	                      ResObj_Convert, &h))
		return NULL;
	_rv = as_Menu(h);
	_res = Py_BuildValue("O&",
	                     MenuObj_New, _rv);
	return _res;
}

static PyObject *Menu_GetMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	MenuHandle _rv;
	short resourceID;
#ifndef GetMenu
	PyMac_PRECHECK(GetMenu);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &resourceID))
		return NULL;
	_rv = GetMenu(resourceID);
	_res = Py_BuildValue("O&",
	                     MenuObj_New, _rv);
	return _res;
}

static PyObject *Menu_DeleteMenu(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
	short menuID;
#ifndef DeleteMenu
	PyMac_PRECHECK(DeleteMenu);
#endif
	if (!PyArg_ParseTuple(_args, "h",
	                      &menuID))
		return NULL;
	DeleteMenu(menuID);
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyObject *Menu_DrawMenuBar(PyObject *_self, PyObject *_args)
{
	PyObject *_res = NULL;
#ifndef DrawMenuBar
	PyMac_PRECHECK(DrawMenuBar);
#endif
	if (!PyArg_ParseTuple(_args, ""))
		return NULL;
	DrawMenuBar();
	Py_INCREF(Py_None);
	_res = Py_None;
	return _res;
}

static PyMethodDef Menu_methods[] = {

#if !TARGET_API_MAC_CARBON
	{"InitProcMenu", (PyCFunction)Menu_InitProcMenu, 1,
	 "(short resID) -> None"},
#endif

#if !TARGET_API_MAC_CARBON
	{"InitMenus", (PyCFunction)Menu_InitMenus, 1,
	 "() -> None"},
#endif
	{"NewMenu", (PyCFunction)Menu_NewMenu, 1,
	 "(MenuID menuID, Str255 menuTitle) -> (MenuHandle _rv)"},
	{"MacGetMenu", (PyCFunction)Menu_MacGetMenu, 1,
	 "(short resourceID) -> (MenuHandle _rv)"},

#if TARGET_API_MAC_CARBON
	{"CreateNewMenu", (PyCFunction)Menu_CreateNewMenu, 1,
	 "(MenuID inMenuID, MenuAttributes inMenuAttributes) -> (MenuHandle outMenuRef)"},
#endif
	{"MenuKey", (PyCFunction)Menu_MenuKey, 1,
	 "(CharParameter ch) -> (long _rv)"},
	{"MenuSelect", (PyCFunction)Menu_MenuSelect, 1,
	 "(Point startPt) -> (long _rv)"},
	{"MenuChoice", (PyCFunction)Menu_MenuChoice, 1,
	 "() -> (long _rv)"},
	{"MenuEvent", (PyCFunction)Menu_MenuEvent, 1,
	 "(EventRecord inEvent) -> (UInt32 _rv)"},
	{"GetMBarHeight", (PyCFunction)Menu_GetMBarHeight, 1,
	 "() -> (short _rv)"},
	{"MacDrawMenuBar", (PyCFunction)Menu_MacDrawMenuBar, 1,
	 "() -> None"},
	{"InvalMenuBar", (PyCFunction)Menu_InvalMenuBar, 1,
	 "() -> None"},
	{"HiliteMenu", (PyCFunction)Menu_HiliteMenu, 1,
	 "(MenuID menuID) -> None"},
	{"GetNewMBar", (PyCFunction)Menu_GetNewMBar, 1,
	 "(short menuBarID) -> (MenuBarHandle _rv)"},
	{"GetMenuBar", (PyCFunction)Menu_GetMenuBar, 1,
	 "() -> (MenuBarHandle _rv)"},
	{"SetMenuBar", (PyCFunction)Menu_SetMenuBar, 1,
	 "(MenuBarHandle mbar) -> None"},

#if TARGET_API_MAC_CARBON
	{"DuplicateMenuBar", (PyCFunction)Menu_DuplicateMenuBar, 1,
	 "(MenuBarHandle inMbar) -> (MenuBarHandle outMbar)"},
#endif

#if TARGET_API_MAC_CARBON
	{"DisposeMenuBar", (PyCFunction)Menu_DisposeMenuBar, 1,
	 "(MenuBarHandle inMbar) -> None"},
#endif
	{"GetMenuHandle", (PyCFunction)Menu_GetMenuHandle, 1,
	 "(MenuID menuID) -> (MenuHandle _rv)"},
	{"MacDeleteMenu", (PyCFunction)Menu_MacDeleteMenu, 1,
	 "(MenuID menuID) -> None"},
	{"ClearMenuBar", (PyCFunction)Menu_ClearMenuBar, 1,
	 "() -> None"},

#if !TARGET_API_MAC_CARBON
	{"SetMenuFlash", (PyCFunction)Menu_SetMenuFlash, 1,
	 "(short count) -> None"},
#endif
	{"SetMenuFlashCount", (PyCFunction)Menu_SetMenuFlashCount, 1,
	 "(short count) -> None"},
	{"FlashMenuBar", (PyCFunction)Menu_FlashMenuBar, 1,
	 "(MenuID menuID) -> None"},

#if !TARGET_API_MAC_CARBON
	{"SystemEdit", (PyCFunction)Menu_SystemEdit, 1,
	 "(short editCmd) -> (Boolean _rv)"},
#endif

#if !TARGET_API_MAC_CARBON
	{"SystemMenu", (PyCFunction)Menu_SystemMenu, 1,
	 "(long menuResult) -> None"},
#endif
	{"IsMenuBarVisible", (PyCFunction)Menu_IsMenuBarVisible, 1,
	 "() -> (Boolean _rv)"},
	{"ShowMenuBar", (PyCFunction)Menu_ShowMenuBar, 1,
	 "() -> None"},
	{"HideMenuBar", (PyCFunction)Menu_HideMenuBar, 1,
	 "() -> None"},

#if TARGET_API_MAC_CARBON
	{"AcquireRootMenu", (PyCFunction)Menu_AcquireRootMenu, 1,
	 "() -> (MenuHandle _rv)"},
#endif
	{"DeleteMCEntries", (PyCFunction)Menu_DeleteMCEntries, 1,
	 "(MenuID menuID, short menuItem) -> None"},
	{"InitContextualMenus", (PyCFunction)Menu_InitContextualMenus, 1,
	 "() -> None"},
	{"IsShowContextualMenuClick", (PyCFunction)Menu_IsShowContextualMenuClick, 1,
	 "(EventRecord inEvent) -> (Boolean _rv)"},
	{"LMGetTheMenu", (PyCFunction)Menu_LMGetTheMenu, 1,
	 "() -> (SInt16 _rv)"},

#if !TARGET_API_MAC_CARBON
	{"OpenDeskAcc", (PyCFunction)Menu_OpenDeskAcc, 1,
	 "(Str255 name) -> None"},
#endif
	{"as_Menu", (PyCFunction)Menu_as_Menu, 1,
	 "(Handle h) -> (MenuHandle _rv)"},
	{"GetMenu", (PyCFunction)Menu_GetMenu, 1,
	 "(short resourceID) -> (MenuHandle _rv)"},
	{"DeleteMenu", (PyCFunction)Menu_DeleteMenu, 1,
	 "(short menuID) -> None"},
	{"DrawMenuBar", (PyCFunction)Menu_DrawMenuBar, 1,
	 "() -> None"},
	{NULL, NULL, 0}
};




void init_Menu(void)
{
	PyObject *m;
	PyObject *d;



		PyMac_INIT_TOOLBOX_OBJECT_NEW(MenuHandle, MenuObj_New);
		PyMac_INIT_TOOLBOX_OBJECT_CONVERT(MenuHandle, MenuObj_Convert);


	m = Py_InitModule("_Menu", Menu_methods);
	d = PyModule_GetDict(m);
	Menu_Error = PyMac_GetOSErrException();
	if (Menu_Error == NULL ||
	    PyDict_SetItemString(d, "Error", Menu_Error) != 0)
		return;
	Menu_Type.ob_type = &PyType_Type;
	Py_INCREF(&Menu_Type);
	if (PyDict_SetItemString(d, "MenuType", (PyObject *)&Menu_Type) != 0)
		Py_FatalError("can't initialize MenuType");
}

/* ======================== End module _Menu ======================== */

