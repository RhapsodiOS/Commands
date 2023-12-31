
/* Generic object operations; and implementation of None (NoObject) */

#include "Python.h"

#ifdef macintosh
#include "macglue.h"
#endif

/* just for trashcan: */
#include "compile.h"
#include "frameobject.h"
#include "traceback.h"

#if defined( Py_TRACE_REFS ) || defined( Py_REF_DEBUG )
DL_IMPORT(long) _Py_RefTotal;
#endif

DL_IMPORT(int) Py_DivisionWarningFlag;

/* Object allocation routines used by NEWOBJ and NEWVAROBJ macros.
   These are used by the individual routines for object creation.
   Do not call them otherwise, they do not initialize the object! */

#ifdef COUNT_ALLOCS
static PyTypeObject *type_list;
extern int tuple_zero_allocs, fast_tuple_allocs;
extern int quick_int_allocs, quick_neg_int_allocs;
extern int null_strings, one_strings;
void
dump_counts(void)
{
	PyTypeObject *tp;

	for (tp = type_list; tp; tp = tp->tp_next)
		fprintf(stderr, "%s alloc'd: %d, freed: %d, max in use: %d\n",
			tp->tp_name, tp->tp_allocs, tp->tp_frees,
			tp->tp_maxalloc);
	fprintf(stderr, "fast tuple allocs: %d, empty: %d\n",
		fast_tuple_allocs, tuple_zero_allocs);
	fprintf(stderr, "fast int allocs: pos: %d, neg: %d\n",
		quick_int_allocs, quick_neg_int_allocs);
	fprintf(stderr, "null strings: %d, 1-strings: %d\n",
		null_strings, one_strings);
}

PyObject *
get_counts(void)
{
	PyTypeObject *tp;
	PyObject *result;
	PyObject *v;

	result = PyList_New(0);
	if (result == NULL)
		return NULL;
	for (tp = type_list; tp; tp = tp->tp_next) {
		v = Py_BuildValue("(siii)", tp->tp_name, tp->tp_allocs,
				  tp->tp_frees, tp->tp_maxalloc);
		if (v == NULL) {
			Py_DECREF(result);
			return NULL;
		}
		if (PyList_Append(result, v) < 0) {
			Py_DECREF(v);
			Py_DECREF(result);
			return NULL;
		}
		Py_DECREF(v);
	}
	return result;
}

void
inc_count(PyTypeObject *tp)
{
	if (tp->tp_allocs == 0) {
		/* first time; insert in linked list */
		if (tp->tp_next != NULL) /* sanity check */
			Py_FatalError("XXX inc_count sanity check");
		tp->tp_next = type_list;
		type_list = tp;
	}
	tp->tp_allocs++;
	if (tp->tp_allocs - tp->tp_frees > tp->tp_maxalloc)
		tp->tp_maxalloc = tp->tp_allocs - tp->tp_frees;
}
#endif

PyObject *
PyObject_Init(PyObject *op, PyTypeObject *tp)
{
	if (op == NULL) {
		PyErr_SetString(PyExc_SystemError,
				"NULL object passed to PyObject_Init");
		return op;
  	}
	/* Any changes should be reflected in PyObject_INIT (objimpl.h) */
	op->ob_type = tp;
	_Py_NewReference(op);
	return op;
}

PyVarObject *
PyObject_InitVar(PyVarObject *op, PyTypeObject *tp, int size)
{
	if (op == NULL) {
		PyErr_SetString(PyExc_SystemError,
				"NULL object passed to PyObject_InitVar");
		return op;
	}
	/* Any changes should be reflected in PyObject_INIT_VAR */
	op->ob_size = size;
	op->ob_type = tp;
	_Py_NewReference((PyObject *)op);
	return op;
}

PyObject *
_PyObject_New(PyTypeObject *tp)
{
	PyObject *op;
	op = (PyObject *) PyObject_MALLOC(_PyObject_SIZE(tp));
	if (op == NULL)
		return PyErr_NoMemory();
	return PyObject_INIT(op, tp);
}

PyVarObject *
_PyObject_NewVar(PyTypeObject *tp, int nitems)
{
	PyVarObject *op;
	const size_t size = _PyObject_VAR_SIZE(tp, nitems);
	op = (PyVarObject *) PyObject_MALLOC(size);
	if (op == NULL)
		return (PyVarObject *)PyErr_NoMemory();
	return PyObject_INIT_VAR(op, tp, nitems);
}

void
_PyObject_Del(PyObject *op)
{
	PyObject_FREE(op);
}

int
PyObject_Print(PyObject *op, FILE *fp, int flags)
{
	int ret = 0;
	if (PyErr_CheckSignals())
		return -1;
#ifdef USE_STACKCHECK
	if (PyOS_CheckStack()) {
		PyErr_SetString(PyExc_MemoryError, "stack overflow");
		return -1;
	}
#endif
	clearerr(fp); /* Clear any previous error condition */
	if (op == NULL) {
		fprintf(fp, "<nil>");
	}
	else {
		if (op->ob_refcnt <= 0)
			fprintf(fp, "<refcnt %u at %p>",
				op->ob_refcnt, op);
		else if (op->ob_type->tp_print == NULL) {
			PyObject *s;
			if (flags & Py_PRINT_RAW)
				s = PyObject_Str(op);
			else
				s = PyObject_Repr(op);
			if (s == NULL)
				ret = -1;
			else {
				ret = PyObject_Print(s, fp, Py_PRINT_RAW);
			}
			Py_XDECREF(s);
		}
		else
			ret = (*op->ob_type->tp_print)(op, fp, flags);
	}
	if (ret == 0) {
		if (ferror(fp)) {
			PyErr_SetFromErrno(PyExc_IOError);
			clearerr(fp);
			ret = -1;
		}
	}
	return ret;
}

/* For debugging convenience.  See Misc/gdbinit for some useful gdb hooks */
void _PyObject_Dump(PyObject* op) 
{
	if (op == NULL)
		fprintf(stderr, "NULL\n");
	else {
		fprintf(stderr, "object  : ");
		(void)PyObject_Print(op, stderr, 0);
		fprintf(stderr, "\n"
			"type    : %s\n"
			"refcount: %d\n"
			"address : %p\n",
			op->ob_type==NULL ? "NULL" : op->ob_type->tp_name,
			op->ob_refcnt,
			op);
	}
}

PyObject *
PyObject_Repr(PyObject *v)
{
	if (PyErr_CheckSignals())
		return NULL;
#ifdef USE_STACKCHECK
	if (PyOS_CheckStack()) {
		PyErr_SetString(PyExc_MemoryError, "stack overflow");
		return NULL;
	}
#endif
	if (v == NULL)
		return PyString_FromString("<NULL>");
	else if (v->ob_type->tp_repr == NULL)
		return PyString_FromFormat("<%s object at %p>",
					   v->ob_type->tp_name, v);
	else {
		PyObject *res;
		res = (*v->ob_type->tp_repr)(v);
		if (res == NULL)
			return NULL;
#ifdef Py_USING_UNICODE
		if (PyUnicode_Check(res)) {
			PyObject* str;
			str = PyUnicode_AsUnicodeEscapeString(res);
			Py_DECREF(res);
			if (str)
				res = str;
			else
				return NULL;
		}
#endif
		if (!PyString_Check(res)) {
			PyErr_Format(PyExc_TypeError,
				     "__repr__ returned non-string (type %.200s)",
				     res->ob_type->tp_name);
			Py_DECREF(res);
			return NULL;
		}
		return res;
	}
}

PyObject *
PyObject_Str(PyObject *v)
{
	PyObject *res;
	
	if (v == NULL)
		return PyString_FromString("<NULL>");
	if (PyString_CheckExact(v)) {
		Py_INCREF(v);
		return v;
	}
	if (v->ob_type->tp_str == NULL)
		return PyObject_Repr(v);

	res = (*v->ob_type->tp_str)(v);
	if (res == NULL)
		return NULL;
#ifdef Py_USING_UNICODE
	if (PyUnicode_Check(res)) {
		PyObject* str;
		str = PyUnicode_AsEncodedString(res, NULL, NULL);
		Py_DECREF(res);
		if (str)
			res = str;
		else
		    	return NULL;
	}
#endif
	if (!PyString_Check(res)) {
		PyErr_Format(PyExc_TypeError,
			     "__str__ returned non-string (type %.200s)",
			     res->ob_type->tp_name);
		Py_DECREF(res);
		return NULL;
	}
	return res;
}

#ifdef Py_USING_UNICODE
PyObject *
PyObject_Unicode(PyObject *v)
{
	PyObject *res;
	
	if (v == NULL)
		res = PyString_FromString("<NULL>");
	if (PyUnicode_CheckExact(v)) {
		Py_INCREF(v);
		return v;
	}
	if (PyUnicode_Check(v)) {
		/* For a Unicode subtype that's not a Unicode object,
		   return a true Unicode object with the same data. */
		return PyUnicode_FromUnicode(PyUnicode_AS_UNICODE(v),
					     PyUnicode_GET_SIZE(v));
	}
	if (PyString_Check(v)) {
		Py_INCREF(v);
	    	res = v;
    	}
	else {
		PyObject *func;
		static PyObject *unicodestr;
		/* XXX As soon as we have a tp_unicode slot, we should
		       check this before trying the __unicode__
		       method. */
		if (unicodestr == NULL) {
			unicodestr= PyString_InternFromString(
						       "__unicode__");
			if (unicodestr == NULL)
				return NULL;
		}
		func = PyObject_GetAttr(v, unicodestr);
		if (func != NULL) {
		    	res = PyEval_CallObject(func, (PyObject *)NULL);
			Py_DECREF(func);
		}
		else {
			PyErr_Clear();
			if (v->ob_type->tp_str != NULL)
				res = (*v->ob_type->tp_str)(v);
			else
				res = PyObject_Repr(v);
		}
	}
	if (res == NULL)
		return NULL;
	if (!PyUnicode_Check(res)) {
		PyObject *str;
		str = PyUnicode_FromEncodedObject(res, NULL, "strict");
		Py_DECREF(res);
		if (str)
			res = str;
		else
		    	return NULL;
	}
	return res;
}
#endif


/* Macro to get the tp_richcompare field of a type if defined */
#define RICHCOMPARE(t) (PyType_HasFeature((t), Py_TPFLAGS_HAVE_RICHCOMPARE) \
                         ? (t)->tp_richcompare : NULL)

/* Map rich comparison operators to their swapped version, e.g. LT --> GT */
static int swapped_op[] = {Py_GT, Py_GE, Py_EQ, Py_NE, Py_LT, Py_LE};

/* Try a genuine rich comparison, returning an object.  Return:
   NULL for exception;
   NotImplemented if this particular rich comparison is not implemented or
     undefined;
   some object not equal to NotImplemented if it is implemented
     (this latter object may not be a Boolean).
*/
static PyObject *
try_rich_compare(PyObject *v, PyObject *w, int op)
{
	richcmpfunc f;
	PyObject *res;

	if (v->ob_type != w->ob_type &&
	    PyType_IsSubtype(w->ob_type, v->ob_type) &&
	    (f = RICHCOMPARE(w->ob_type)) != NULL) {
		res = (*f)(w, v, swapped_op[op]);
		if (res != Py_NotImplemented)
			return res;
		Py_DECREF(res);
	}
	if ((f = RICHCOMPARE(v->ob_type)) != NULL) {
		res = (*f)(v, w, op);
		if (res != Py_NotImplemented)
			return res;
		Py_DECREF(res);
	}
	if ((f = RICHCOMPARE(w->ob_type)) != NULL) {
		return (*f)(w, v, swapped_op[op]);
	}
	res = Py_NotImplemented;
	Py_INCREF(res);
	return res;
}

/* Try a genuine rich comparison, returning an int.  Return:
   -1 for exception (including the case where try_rich_compare() returns an
      object that's not a Boolean);
    0 if the outcome is false;
    1 if the outcome is true;
    2 if this particular rich comparison is not implemented or undefined.
*/
static int
try_rich_compare_bool(PyObject *v, PyObject *w, int op)
{
	PyObject *res;
	int ok;

	if (RICHCOMPARE(v->ob_type) == NULL && RICHCOMPARE(w->ob_type) == NULL)
		return 2; /* Shortcut, avoid INCREF+DECREF */
	res = try_rich_compare(v, w, op);
	if (res == NULL)
		return -1;
	if (res == Py_NotImplemented) {
		Py_DECREF(res);
		return 2;
	}
	ok = PyObject_IsTrue(res);
	Py_DECREF(res);
	return ok;
}

/* Try rich comparisons to determine a 3-way comparison.  Return:
   -2 for an exception;
   -1 if v  < w;
    0 if v == w;
    1 if v  > w;
    2 if this particular rich comparison is not implemented or undefined.
*/
static int
try_rich_to_3way_compare(PyObject *v, PyObject *w)
{
	static struct { int op; int outcome; } tries[3] = {
		/* Try this operator, and if it is true, use this outcome: */
		{Py_EQ, 0},
		{Py_LT, -1},
		{Py_GT, 1},
	};
	int i;

	if (RICHCOMPARE(v->ob_type) == NULL && RICHCOMPARE(w->ob_type) == NULL)
		return 2; /* Shortcut */

	for (i = 0; i < 3; i++) {
		switch (try_rich_compare_bool(v, w, tries[i].op)) {
		case -1:
			return -2;
		case 1:
			return tries[i].outcome;
		}
	}

	return 2;
}

/* Try a 3-way comparison, returning an int.  Return:
   -2 for an exception;
   -1 if v <  w;
    0 if v == w;
    1 if v  > w;
    2 if this particular 3-way comparison is not implemented or undefined.
*/
static int
try_3way_compare(PyObject *v, PyObject *w)
{
	int c;
	cmpfunc f;

	/* Comparisons involving instances are given to instance_compare,
	   which has the same return conventions as this function. */

	f = v->ob_type->tp_compare;
	if (PyInstance_Check(v))
		return (*f)(v, w);
	if (PyInstance_Check(w))
		return (*w->ob_type->tp_compare)(v, w);

	/* If both have the same (non-NULL) tp_compare, use it. */
	if (f != NULL && f == w->ob_type->tp_compare) {
		c = (*f)(v, w);
		if (c < 0 && PyErr_Occurred())
			return -1;
		return c < 0 ? -1 : c > 0 ? 1 : 0;
	}

	/* If either tp_compare is _PyObject_SlotCompare, that's safe. */
	if (f == _PyObject_SlotCompare ||
	    w->ob_type->tp_compare == _PyObject_SlotCompare)
		return _PyObject_SlotCompare(v, w);

	/* Try coercion; if it fails, give up */
	c = PyNumber_CoerceEx(&v, &w);
	if (c < 0)
		return -2;
	if (c > 0)
		return 2;

	/* Try v's comparison, if defined */
	if ((f = v->ob_type->tp_compare) != NULL) {
		c = (*f)(v, w);
		Py_DECREF(v);
		Py_DECREF(w);
		if (c < 0 && PyErr_Occurred())
			return -2;
		return c < 0 ? -1 : c > 0 ? 1 : 0;
	}

	/* Try w's comparison, if defined */
	if ((f = w->ob_type->tp_compare) != NULL) {
		c = (*f)(w, v); /* swapped! */
		Py_DECREF(v);
		Py_DECREF(w);
		if (c < 0 && PyErr_Occurred())
			return -2;
		return c < 0 ? 1 : c > 0 ? -1 : 0; /* negated! */
	}

	/* No comparison defined */
	Py_DECREF(v);
	Py_DECREF(w);
	return 2;
}

/* Final fallback 3-way comparison, returning an int.  Return:
   -2 if an error occurred;
   -1 if v <  w;
    0 if v == w;
    1 if v >  w.
*/
static int
default_3way_compare(PyObject *v, PyObject *w)
{
	int c;
	char *vname, *wname;

	if (v->ob_type == w->ob_type) {
		/* When comparing these pointers, they must be cast to
		 * integer types (i.e. Py_uintptr_t, our spelling of C9X's
		 * uintptr_t).  ANSI specifies that pointer compares other
		 * than == and != to non-related structures are undefined.
		 */
		Py_uintptr_t vv = (Py_uintptr_t)v;
		Py_uintptr_t ww = (Py_uintptr_t)w;
		return (vv < ww) ? -1 : (vv > ww) ? 1 : 0;
	}

#ifdef Py_USING_UNICODE
	/* Special case for Unicode */
	if (PyUnicode_Check(v) || PyUnicode_Check(w)) {
		c = PyUnicode_Compare(v, w);
		if (!PyErr_Occurred())
			return c;
		/* TypeErrors are ignored: if Unicode coercion fails due
		   to one of the arguments not having the right type, we
		   continue as defined by the coercion protocol (see
		   above).  Luckily, decoding errors are reported as
		   ValueErrors and are not masked by this technique. */
		if (!PyErr_ExceptionMatches(PyExc_TypeError))
			return -2;
		PyErr_Clear();
	}
#endif

	/* None is smaller than anything */
	if (v == Py_None)
		return -1;
	if (w == Py_None)
		return 1;

	/* different type: compare type names */
	if (v->ob_type->tp_as_number)
		vname = "";
	else
		vname = v->ob_type->tp_name;
	if (w->ob_type->tp_as_number)
		wname = "";
	else
		wname = w->ob_type->tp_name;
	c = strcmp(vname, wname);
	if (c < 0)
		return -1;
	if (c > 0)
		return 1;
	/* Same type name, or (more likely) incomparable numeric types */
	return ((Py_uintptr_t)(v->ob_type) < (
		Py_uintptr_t)(w->ob_type)) ? -1 : 1;
}

#define CHECK_TYPES(o) PyType_HasFeature((o)->ob_type, Py_TPFLAGS_CHECKTYPES)

/* Do a 3-way comparison, by hook or by crook.  Return:
   -2 for an exception;
   -1 if v <  w;
    0 if v == w;
    1 if v >  w;
   If the object implements a tp_compare function, it returns
   whatever this function returns (whether with an exception or not).
*/
static int
do_cmp(PyObject *v, PyObject *w)
{
	int c;
	cmpfunc f;

	if (v->ob_type == w->ob_type
	    && (f = v->ob_type->tp_compare) != NULL) {
		c = (*f)(v, w);
		if (c != 2 || !PyInstance_Check(v))
			return c;
	}
	c = try_rich_to_3way_compare(v, w);
	if (c < 2)
		return c;
	c = try_3way_compare(v, w);
	if (c < 2)
		return c;
	return default_3way_compare(v, w);
}

/* compare_nesting is incremented before calling compare (for
   some types) and decremented on exit.  If the count exceeds the
   nesting limit, enable code to detect circular data structures.

   This is a tunable parameter that should only affect the performance
   of comparisons, nothing else.  Setting it high makes comparing deeply
   nested non-cyclical data structures faster, but makes comparing cyclical
   data structures slower.
*/
#define NESTING_LIMIT 20

static int compare_nesting = 0;

static PyObject*
get_inprogress_dict(void)
{
	static PyObject *key;
	PyObject *tstate_dict, *inprogress;

	if (key == NULL) {
		key = PyString_InternFromString("cmp_state");
		if (key == NULL)
			return NULL;
	}

	tstate_dict = PyThreadState_GetDict();
	if (tstate_dict == NULL) {
		PyErr_BadInternalCall();
		return NULL;
	} 

	inprogress = PyDict_GetItem(tstate_dict, key); 
	if (inprogress == NULL) {
		inprogress = PyDict_New();
		if (inprogress == NULL)
			return NULL;
		if (PyDict_SetItem(tstate_dict, key, inprogress) == -1) {
		    Py_DECREF(inprogress);
		    return NULL;
		}
		Py_DECREF(inprogress);
	}

	return inprogress;
}

static PyObject *
check_recursion(PyObject *v, PyObject *w, int op)
{
	PyObject *inprogress;
	PyObject *token;
	Py_uintptr_t iv = (Py_uintptr_t)v;
	Py_uintptr_t iw = (Py_uintptr_t)w;
	PyObject *x, *y, *z;

	inprogress = get_inprogress_dict();
	if (inprogress == NULL)
		return NULL;

	token = PyTuple_New(3);
	if (token == NULL)
		return NULL;

	if (iv <= iw) {
		PyTuple_SET_ITEM(token, 0, x = PyLong_FromVoidPtr((void *)v));
		PyTuple_SET_ITEM(token, 1, y = PyLong_FromVoidPtr((void *)w));
		if (op >= 0)
			op = swapped_op[op];
	} else {
		PyTuple_SET_ITEM(token, 0, x = PyLong_FromVoidPtr((void *)w));
		PyTuple_SET_ITEM(token, 1, y = PyLong_FromVoidPtr((void *)v));
	}
	PyTuple_SET_ITEM(token, 2, z = PyInt_FromLong((long)op));
	if (x == NULL || y == NULL || z == NULL) {
		Py_DECREF(token);
		return NULL;
	}

	if (PyDict_GetItem(inprogress, token) != NULL) {
		Py_DECREF(token);
		return Py_None; /* Without INCREF! */
	}

	if (PyDict_SetItem(inprogress, token, token) < 0) {
		Py_DECREF(token);
		return NULL;
	}

	return token;
}

static void
delete_token(PyObject *token)
{
	PyObject *inprogress;

	if (token == NULL || token == Py_None)
		return;
	inprogress = get_inprogress_dict();
	if (inprogress == NULL)
		PyErr_Clear();
	else
		PyDict_DelItem(inprogress, token);
	Py_DECREF(token);
}

/* Compare v to w.  Return
   -1 if v <  w or exception (PyErr_Occurred() true in latter case).
    0 if v == w.
    1 if v > w.
   XXX The docs (C API manual) say the return value is undefined in case
   XXX of error.
*/
int
PyObject_Compare(PyObject *v, PyObject *w)
{
	PyTypeObject *vtp;
	int result;

#if defined(USE_STACKCHECK)
	if (PyOS_CheckStack()) {
		PyErr_SetString(PyExc_MemoryError, "Stack overflow");
		return -1;
	}
#endif
	if (v == NULL || w == NULL) {
		PyErr_BadInternalCall();
		return -1;
	}
	if (v == w)
		return 0;
	vtp = v->ob_type;
	compare_nesting++;
	if (compare_nesting > NESTING_LIMIT &&
		(vtp->tp_as_mapping
		 || (vtp->tp_as_sequence
		     && !PyString_Check(v)
		     && !PyTuple_Check(v)))) {
		/* try to detect circular data structures */
		PyObject *token = check_recursion(v, w, -1);

		if (token == NULL) {
			result = -1;
		}
		else if (token == Py_None) {
			/* already comparing these objects.  assume
			   they're equal until shown otherwise */
                        result = 0;
		}
		else {
			result = do_cmp(v, w);
			delete_token(token);
		}
	}
	else {
		result = do_cmp(v, w);
	}
	compare_nesting--;
	return result < 0 ? -1 : result;
}

/* Return (new reference to) Py_True or Py_False. */
static PyObject *
convert_3way_to_object(int op, int c)
{
	PyObject *result;
	switch (op) {
	case Py_LT: c = c <  0; break;
	case Py_LE: c = c <= 0; break;
	case Py_EQ: c = c == 0; break;
	case Py_NE: c = c != 0; break;
	case Py_GT: c = c >  0; break;
	case Py_GE: c = c >= 0; break;
	}
	result = c ? Py_True : Py_False;
	Py_INCREF(result);
	return result;
}
	
/* We want a rich comparison but don't have one.  Try a 3-way cmp instead.
   Return
   NULL      if error
   Py_True   if v op w
   Py_False  if not (v op w)
*/
static PyObject *
try_3way_to_rich_compare(PyObject *v, PyObject *w, int op)
{
	int c;

	c = try_3way_compare(v, w);
	if (c >= 2)
		c = default_3way_compare(v, w);
	if (c <= -2)
		return NULL;
	return convert_3way_to_object(op, c);
}

/* Do rich comparison on v and w.  Return
   NULL      if error
   Else a new reference to an object other than Py_NotImplemented, usually(?):
   Py_True   if v op w
   Py_False  if not (v op w)
*/
static PyObject *
do_richcmp(PyObject *v, PyObject *w, int op)
{
	PyObject *res;

	res = try_rich_compare(v, w, op);
	if (res != Py_NotImplemented)
		return res;
	Py_DECREF(res);

	return try_3way_to_rich_compare(v, w, op);
}

/* Return:
   NULL for exception;
   some object not equal to NotImplemented if it is implemented
     (this latter object may not be a Boolean).
*/
PyObject *
PyObject_RichCompare(PyObject *v, PyObject *w, int op)
{
	PyObject *res;

	assert(Py_LT <= op && op <= Py_GE);

	compare_nesting++;
	if (compare_nesting > NESTING_LIMIT &&
		(v->ob_type->tp_as_mapping
		 || (v->ob_type->tp_as_sequence
		     && !PyString_Check(v)
		     && !PyTuple_Check(v)))) {

		/* try to detect circular data structures */
		PyObject *token = check_recursion(v, w, op);
		if (token == NULL) {
			res = NULL;
			goto Done;
		}
		else if (token == Py_None) {
			/* already comparing these objects with this operator.
			   assume they're equal until shown otherwise */
			if (op == Py_EQ)
				res = Py_True;
			else if (op == Py_NE)
				res = Py_False;
			else {
				PyErr_SetString(PyExc_ValueError,
					"can't order recursive values");
				res = NULL;
			}
			Py_XINCREF(res);
		}
		else {
			res = do_richcmp(v, w, op);
			delete_token(token);
		}
		goto Done;
	}

	/* No nesting extremism.
	   If the types are equal, and not old-style instances, try to
	   get out cheap (don't bother with coercions etc.). */
	if (v->ob_type == w->ob_type && !PyInstance_Check(v)) {
		cmpfunc fcmp;
		richcmpfunc frich = RICHCOMPARE(v->ob_type);
		/* If the type has richcmp, try it first.  try_rich_compare
		   tries it two-sided, which is not needed since we've a
		   single type only. */
		if (frich != NULL) {
			res = (*frich)(v, w, op);
			if (res != Py_NotImplemented)
				goto Done;
			Py_DECREF(res);
		}
		/* No richcmp, or this particular richmp not implemented.
		   Try 3-way cmp. */
		fcmp = v->ob_type->tp_compare;
		if (fcmp != NULL) {
			int c = (*fcmp)(v, w);
			if (c < 0 && PyErr_Occurred()) {
				res = NULL;
				goto Done;
			}
			res = convert_3way_to_object(op, c);
			goto Done;
		}
	}

	/* Fast path not taken, or couldn't deliver a useful result. */
	res = do_richcmp(v, w, op);
Done:
	compare_nesting--;
	return res;
}

/* Return -1 if error; 1 if v op w; 0 if not (v op w). */
int
PyObject_RichCompareBool(PyObject *v, PyObject *w, int op)
{
	PyObject *res = PyObject_RichCompare(v, w, op);
	int ok;

	if (res == NULL)
		return -1;
	ok = PyObject_IsTrue(res);
	Py_DECREF(res);
	return ok;
}

/* Set of hash utility functions to help maintaining the invariant that
	iff a==b then hash(a)==hash(b)

   All the utility functions (_Py_Hash*()) return "-1" to signify an error.
*/

long
_Py_HashDouble(double v)
{
	double intpart, fractpart;
	int expo;
	long hipart;
	long x;		/* the final hash value */
	/* This is designed so that Python numbers of different types
	 * that compare equal hash to the same value; otherwise comparisons
	 * of mapping keys will turn out weird.
	 */

#ifdef MPW /* MPW C modf expects pointer to extended as second argument */
{
	extended e;
	fractpart = modf(v, &e);
	intpart = e;
}
#else
	fractpart = modf(v, &intpart);
#endif
	if (fractpart == 0.0) {
		/* This must return the same hash as an equal int or long. */
		if (intpart > LONG_MAX || -intpart > LONG_MAX) {
			/* Convert to long and use its hash. */
			PyObject *plong;	/* converted to Python long */
			if (Py_IS_INFINITY(intpart))
				/* can't convert to long int -- arbitrary */
				v = v < 0 ? -271828.0 : 314159.0;
			plong = PyLong_FromDouble(v);
			if (plong == NULL)
				return -1;
			x = PyObject_Hash(plong);
			Py_DECREF(plong);
			return x;
		}
		/* Fits in a C long == a Python int, so is its own hash. */
		x = (long)intpart;
		if (x == -1)
			x = -2;
		return x;
	}
	/* The fractional part is non-zero, so we don't have to worry about
	 * making this match the hash of some other type.
	 * Use frexp to get at the bits in the double.
	 * Since the VAX D double format has 56 mantissa bits, which is the
	 * most of any double format in use, each of these parts may have as
	 * many as (but no more than) 56 significant bits.
	 * So, assuming sizeof(long) >= 4, each part can be broken into two
	 * longs; frexp and multiplication are used to do that.
	 * Also, since the Cray double format has 15 exponent bits, which is
	 * the most of any double format in use, shifting the exponent field
	 * left by 15 won't overflow a long (again assuming sizeof(long) >= 4).
	 */
	v = frexp(v, &expo);
	v *= 2147483648.0;	/* 2**31 */
	hipart = (long)v;	/* take the top 32 bits */
	v = (v - (double)hipart) * 2147483648.0; /* get the next 32 bits */
	x = hipart + (long)v + (expo << 15);
	if (x == -1)
		x = -2;
	return x;
}

long
_Py_HashPointer(void *p)
{
#if SIZEOF_LONG >= SIZEOF_VOID_P
	return (long)p;
#else
	/* convert to a Python long and hash that */
	PyObject* longobj;
	long x;
	
	if ((longobj = PyLong_FromVoidPtr(p)) == NULL) {
		x = -1;
		goto finally;
	}
	x = PyObject_Hash(longobj);
	
finally:
	Py_XDECREF(longobj);
	return x;
#endif
}


long
PyObject_Hash(PyObject *v)
{
	PyTypeObject *tp = v->ob_type;
	if (tp->tp_hash != NULL)
		return (*tp->tp_hash)(v);
	if (tp->tp_compare == NULL && RICHCOMPARE(tp) == NULL) {
		return _Py_HashPointer(v); /* Use address as hash value */
	}
	/* If there's a cmp but no hash defined, the object can't be hashed */
	PyErr_SetString(PyExc_TypeError, "unhashable type");
	return -1;
}

PyObject *
PyObject_GetAttrString(PyObject *v, char *name)
{
	PyObject *w, *res;

	if (v->ob_type->tp_getattr != NULL)
		return (*v->ob_type->tp_getattr)(v, name);
	w = PyString_InternFromString(name);
	if (w == NULL)
		return NULL;
	res = PyObject_GetAttr(v, w);
	Py_XDECREF(w);
	return res;
}

int
PyObject_HasAttrString(PyObject *v, char *name)
{
	PyObject *res = PyObject_GetAttrString(v, name);
	if (res != NULL) {
		Py_DECREF(res);
		return 1;
	}
	PyErr_Clear();
	return 0;
}

int
PyObject_SetAttrString(PyObject *v, char *name, PyObject *w)
{
	PyObject *s;
	int res;

	if (v->ob_type->tp_setattr != NULL)
		return (*v->ob_type->tp_setattr)(v, name, w);
	s = PyString_InternFromString(name);
	if (s == NULL)
		return -1;
	res = PyObject_SetAttr(v, s, w);
	Py_XDECREF(s);
	return res;
}

PyObject *
PyObject_GetAttr(PyObject *v, PyObject *name)
{
	PyTypeObject *tp = v->ob_type;

#ifdef Py_USING_UNICODE
	/* The Unicode to string conversion is done here because the
	   existing tp_getattro slots expect a string object as name
	   and we wouldn't want to break those. */
	if (PyUnicode_Check(name)) {
		name = _PyUnicode_AsDefaultEncodedString(name, NULL);
		if (name == NULL)
			return NULL;
	}
	else
#endif
	if (!PyString_Check(name)) {
		PyErr_SetString(PyExc_TypeError,
				"attribute name must be string");
		return NULL;
	}
	if (tp->tp_getattro != NULL)
		return (*tp->tp_getattro)(v, name);
	if (tp->tp_getattr != NULL)
		return (*tp->tp_getattr)(v, PyString_AS_STRING(name));
	PyErr_Format(PyExc_AttributeError,
		     "'%.50s' object has no attribute '%.400s'",
		     tp->tp_name, PyString_AS_STRING(name));
	return NULL;
}

int
PyObject_HasAttr(PyObject *v, PyObject *name)
{
	PyObject *res = PyObject_GetAttr(v, name);
	if (res != NULL) {
		Py_DECREF(res);
		return 1;
	}
	PyErr_Clear();
	return 0;
}

int
PyObject_SetAttr(PyObject *v, PyObject *name, PyObject *value)
{
	PyTypeObject *tp = v->ob_type;
	int err;

#ifdef Py_USING_UNICODE
	/* The Unicode to string conversion is done here because the
	   existing tp_setattro slots expect a string object as name
	   and we wouldn't want to break those. */
	if (PyUnicode_Check(name)) {
		name = PyUnicode_AsEncodedString(name, NULL, NULL);
		if (name == NULL)
			return -1;
	}
	else 
#endif
	if (!PyString_Check(name)){
		PyErr_SetString(PyExc_TypeError,
				"attribute name must be string");
		return -1;
	}
	else
		Py_INCREF(name);

	PyString_InternInPlace(&name);
	if (tp->tp_setattro != NULL) {
		err = (*tp->tp_setattro)(v, name, value);
		Py_DECREF(name);
		return err;
	}
	if (tp->tp_setattr != NULL) {
		err = (*tp->tp_setattr)(v, PyString_AS_STRING(name), value);
		Py_DECREF(name);
		return err;
	}
	Py_DECREF(name);
	if (tp->tp_getattr == NULL && tp->tp_getattro == NULL)
		PyErr_Format(PyExc_TypeError,
			     "'%.100s' object has no attributes "
			     "(%s .%.100s)",
			     tp->tp_name,
			     value==NULL ? "del" : "assign to",
			     PyString_AS_STRING(name));
	else
		PyErr_Format(PyExc_TypeError,
			     "'%.100s' object has only read-only attributes "
			     "(%s .%.100s)",
			     tp->tp_name,
			     value==NULL ? "del" : "assign to",
			     PyString_AS_STRING(name));
	return -1;
}

/* Helper to get a pointer to an object's __dict__ slot, if any */

PyObject **
_PyObject_GetDictPtr(PyObject *obj)
{
	long dictoffset;
	PyTypeObject *tp = obj->ob_type;

	if (!(tp->tp_flags & Py_TPFLAGS_HAVE_CLASS))
		return NULL;
	dictoffset = tp->tp_dictoffset;
	if (dictoffset == 0)
		return NULL;
	if (dictoffset < 0) {
		const size_t size = _PyObject_VAR_SIZE(tp,
					((PyVarObject *)obj)->ob_size);
		dictoffset += (long)size;
		assert(dictoffset > 0);
		assert(dictoffset % SIZEOF_VOID_P == 0);
	}
	return (PyObject **) ((char *)obj + dictoffset);
}

/* Generic GetAttr functions - put these in your tp_[gs]etattro slot */

PyObject *
PyObject_GenericGetAttr(PyObject *obj, PyObject *name)
{
	PyTypeObject *tp = obj->ob_type;
	PyObject *descr;
	PyObject *res = NULL;
	descrgetfunc f;
	PyObject **dictptr;

#ifdef Py_USING_UNICODE
	/* The Unicode to string conversion is done here because the
	   existing tp_setattro slots expect a string object as name
	   and we wouldn't want to break those. */
	if (PyUnicode_Check(name)) {
		name = PyUnicode_AsEncodedString(name, NULL, NULL);
		if (name == NULL)
			return NULL;
	}
	else 
#endif
	if (!PyString_Check(name)){
		PyErr_SetString(PyExc_TypeError,
				"attribute name must be string");
		return NULL;
	}
	else
		Py_INCREF(name);

	if (tp->tp_dict == NULL) {
		if (PyType_Ready(tp) < 0)
			goto done;
	}

	descr = _PyType_Lookup(tp, name);
	f = NULL;
	if (descr != NULL) {
		f = descr->ob_type->tp_descr_get;
		if (f != NULL && PyDescr_IsData(descr)) {
			res = f(descr, obj, (PyObject *)obj->ob_type);
			goto done;
		}
	}

	dictptr = _PyObject_GetDictPtr(obj);
	if (dictptr != NULL) {
		PyObject *dict = *dictptr;
		if (dict != NULL) {
			res = PyDict_GetItem(dict, name);
			if (res != NULL) {
				Py_INCREF(res);
				goto done;
			}
		}
	}

	if (f != NULL) {
		res = f(descr, obj, (PyObject *)obj->ob_type);
		goto done;
	}

	if (descr != NULL) {
		Py_INCREF(descr);
		res = descr;
		goto done;
	}

	PyErr_Format(PyExc_AttributeError,
		     "'%.50s' object has no attribute '%.400s'",
		     tp->tp_name, PyString_AS_STRING(name));
  done:
	Py_DECREF(name);
	return res;
}

int
PyObject_GenericSetAttr(PyObject *obj, PyObject *name, PyObject *value)
{
	PyTypeObject *tp = obj->ob_type;
	PyObject *descr;
	descrsetfunc f;
	PyObject **dictptr;
	int res = -1;

#ifdef Py_USING_UNICODE
	/* The Unicode to string conversion is done here because the
	   existing tp_setattro slots expect a string object as name
	   and we wouldn't want to break those. */
	if (PyUnicode_Check(name)) {
		name = PyUnicode_AsEncodedString(name, NULL, NULL);
		if (name == NULL)
			return -1;
	}
	else 
#endif
	if (!PyString_Check(name)){
		PyErr_SetString(PyExc_TypeError,
				"attribute name must be string");
		return -1;
	}
	else
		Py_INCREF(name);

	if (tp->tp_dict == NULL) {
		if (PyType_Ready(tp) < 0)
			goto done;
	}

	descr = _PyType_Lookup(tp, name);
	f = NULL;
	if (descr != NULL) {
		f = descr->ob_type->tp_descr_set;
		if (f != NULL && PyDescr_IsData(descr)) {
			res = f(descr, obj, value);
			goto done;
		}
	}

	dictptr = _PyObject_GetDictPtr(obj);
	if (dictptr != NULL) {
		PyObject *dict = *dictptr;
		if (dict == NULL && value != NULL) {
			dict = PyDict_New();
			if (dict == NULL)
				goto done;
			*dictptr = dict;
		}
		if (dict != NULL) {
			if (value == NULL)
				res = PyDict_DelItem(dict, name);
			else
				res = PyDict_SetItem(dict, name, value);
			if (res < 0 && PyErr_ExceptionMatches(PyExc_KeyError))
				PyErr_SetObject(PyExc_AttributeError, name);
			goto done;
		}
	}

	if (f != NULL) {
		res = f(descr, obj, value);
		goto done;
	}

	if (descr == NULL) {
		PyErr_Format(PyExc_AttributeError,
			     "'%.50s' object has no attribute '%.400s'",
			     tp->tp_name, PyString_AS_STRING(name));
		goto done;
	}

	PyErr_Format(PyExc_AttributeError,
		     "'%.50s' object attribute '%.400s' is read-only",
		     tp->tp_name, PyString_AS_STRING(name));
  done:
	Py_DECREF(name);
	return res;
}

/* Test a value used as condition, e.g., in a for or if statement.
   Return -1 if an error occurred */

int
PyObject_IsTrue(PyObject *v)
{
	int res;
	if (v == Py_None)
		res = 0;
	else if (v->ob_type->tp_as_number != NULL &&
		 v->ob_type->tp_as_number->nb_nonzero != NULL)
		res = (*v->ob_type->tp_as_number->nb_nonzero)(v);
	else if (v->ob_type->tp_as_mapping != NULL &&
		 v->ob_type->tp_as_mapping->mp_length != NULL)
		res = (*v->ob_type->tp_as_mapping->mp_length)(v);
	else if (v->ob_type->tp_as_sequence != NULL &&
		 v->ob_type->tp_as_sequence->sq_length != NULL)
		res = (*v->ob_type->tp_as_sequence->sq_length)(v);
	else
		res = 1;
	if (res > 0)
		res = 1;
	return res;
}

/* equivalent of 'not v' 
   Return -1 if an error occurred */

int
PyObject_Not(PyObject *v)
{
	int res;
	res = PyObject_IsTrue(v);
	if (res < 0)
		return res;
	return res == 0;
}

/* Coerce two numeric types to the "larger" one.
   Increment the reference count on each argument.
   Return value:
   -1 if an error occurred;
   0 if the coercion succeeded (and then the reference counts are increased);
   1 if no coercion is possible (and no error is raised).
*/
int
PyNumber_CoerceEx(PyObject **pv, PyObject **pw)
{
	register PyObject *v = *pv;
	register PyObject *w = *pw;
	int res;

	if (v->ob_type == w->ob_type && !PyInstance_Check(v)) {
		Py_INCREF(v);
		Py_INCREF(w);
		return 0;
	}
	if (v->ob_type->tp_as_number && v->ob_type->tp_as_number->nb_coerce) {
		res = (*v->ob_type->tp_as_number->nb_coerce)(pv, pw);
		if (res <= 0)
			return res;
	}
	if (w->ob_type->tp_as_number && w->ob_type->tp_as_number->nb_coerce) {
		res = (*w->ob_type->tp_as_number->nb_coerce)(pw, pv);
		if (res <= 0)
			return res;
	}
	return 1;
}

/* Coerce two numeric types to the "larger" one.
   Increment the reference count on each argument.
   Return -1 and raise an exception if no coercion is possible
   (and then no reference count is incremented).
*/
int
PyNumber_Coerce(PyObject **pv, PyObject **pw)
{
	int err = PyNumber_CoerceEx(pv, pw);
	if (err <= 0)
		return err;
	PyErr_SetString(PyExc_TypeError, "number coercion failed");
	return -1;
}


/* Test whether an object can be called */

int
PyCallable_Check(PyObject *x)
{
	if (x == NULL)
		return 0;
	if (PyInstance_Check(x)) {
		PyObject *call = PyObject_GetAttrString(x, "__call__");
		if (call == NULL) {
			PyErr_Clear();
			return 0;
		}
		/* Could test recursively but don't, for fear of endless
		   recursion if some joker sets self.__call__ = self */
		Py_DECREF(call);
		return 1;
	}
	else {
		return x->ob_type->tp_call != NULL;
	}
}

/* Helper for PyObject_Dir.
   Merge the __dict__ of aclass into dict, and recursively also all
   the __dict__s of aclass's base classes.  The order of merging isn't
   defined, as it's expected that only the final set of dict keys is
   interesting.
   Return 0 on success, -1 on error.
*/

static int
merge_class_dict(PyObject* dict, PyObject* aclass)
{
	PyObject *classdict;
	PyObject *bases;

	assert(PyDict_Check(dict));
	assert(aclass);

	/* Merge in the type's dict (if any). */
	classdict = PyObject_GetAttrString(aclass, "__dict__");
	if (classdict == NULL)
		PyErr_Clear();
	else {
		int status = PyDict_Update(dict, classdict);
		Py_DECREF(classdict);
		if (status < 0)
			return -1;
	}

	/* Recursively merge in the base types' (if any) dicts. */
	bases = PyObject_GetAttrString(aclass, "__bases__");
	if (bases == NULL)
		PyErr_Clear();
	else {
		int i, n;
		assert(PyTuple_Check(bases));
		n = PyTuple_GET_SIZE(bases);
		for (i = 0; i < n; i++) {
			PyObject *base = PyTuple_GET_ITEM(bases, i);
			if (merge_class_dict(dict, base) < 0) {
				Py_DECREF(bases);
				return -1;
			}
		}
		Py_DECREF(bases);
	}
	return 0;
}

/* Helper for PyObject_Dir.
   If obj has an attr named attrname that's a list, merge its string
   elements into keys of dict.
   Return 0 on success, -1 on error.  Errors due to not finding the attr,
   or the attr not being a list, are suppressed.
*/

static int
merge_list_attr(PyObject* dict, PyObject* obj, char *attrname)
{
	PyObject *list;
	int result = 0;

	assert(PyDict_Check(dict));
	assert(obj);
	assert(attrname);

	list = PyObject_GetAttrString(obj, attrname);
	if (list == NULL)
		PyErr_Clear();

	else if (PyList_Check(list)) {
		int i;
		for (i = 0; i < PyList_GET_SIZE(list); ++i) {
			PyObject *item = PyList_GET_ITEM(list, i);
			if (PyString_Check(item)) {
				result = PyDict_SetItem(dict, item, Py_None);
				if (result < 0)
					break;
			}
		}
	}

	Py_XDECREF(list);
	return result;
}

/* Like __builtin__.dir(arg).  See bltinmodule.c's builtin_dir for the
   docstring, which should be kept in synch with this implementation. */

PyObject *
PyObject_Dir(PyObject *arg)
{
	/* Set exactly one of these non-NULL before the end. */
	PyObject *result = NULL;	/* result list */
	PyObject *masterdict = NULL;	/* result is masterdict.keys() */

	/* If NULL arg, return the locals. */
	if (arg == NULL) {
		PyObject *locals = PyEval_GetLocals();
		if (locals == NULL)
			goto error;
		result = PyDict_Keys(locals);
		if (result == NULL)
			goto error;
	}

	/* Elif this is some form of module, we only want its dict. */
	else if (PyModule_Check(arg)) {
		masterdict = PyObject_GetAttrString(arg, "__dict__");
		if (masterdict == NULL)
			goto error;
		if (!PyDict_Check(masterdict)) {
			PyErr_SetString(PyExc_TypeError,
					"module.__dict__ is not a dictionary");
			goto error;
		}
	}

	/* Elif some form of type or class, grab its dict and its bases.
	   We deliberately don't suck up its __class__, as methods belonging
	   to the metaclass would probably be more confusing than helpful. */
	else if (PyType_Check(arg) || PyClass_Check(arg)) {
		masterdict = PyDict_New();
		if (masterdict == NULL)
			goto error;
		if (merge_class_dict(masterdict, arg) < 0)
			goto error;
	}

	/* Else look at its dict, and the attrs reachable from its class. */
	else {
		PyObject *itsclass;
		/* Create a dict to start with.  CAUTION:  Not everything
		   responding to __dict__ returns a dict! */
		masterdict = PyObject_GetAttrString(arg, "__dict__");
		if (masterdict == NULL) {
			PyErr_Clear();
			masterdict = PyDict_New();
		}
		else if (!PyDict_Check(masterdict)) {
			Py_DECREF(masterdict);
			masterdict = PyDict_New();
		}
		else {
			/* The object may have returned a reference to its
			   dict, so copy it to avoid mutating it. */
			PyObject *temp = PyDict_Copy(masterdict);
			Py_DECREF(masterdict);
			masterdict = temp;
		}
		if (masterdict == NULL)
			goto error;

		/* Merge in __members__ and __methods__ (if any).
		   XXX Would like this to go away someday; for now, it's
		   XXX needed to get at im_self etc of method objects. */
		if (merge_list_attr(masterdict, arg, "__members__") < 0)
			goto error;
		if (merge_list_attr(masterdict, arg, "__methods__") < 0)
			goto error;

		/* Merge in attrs reachable from its class.
		   CAUTION:  Not all objects have a __class__ attr. */
		itsclass = PyObject_GetAttrString(arg, "__class__");
		if (itsclass == NULL)
			PyErr_Clear();
		else {
			int status = merge_class_dict(masterdict, itsclass);
			Py_DECREF(itsclass);
			if (status < 0)
				goto error;
		}
	}

	assert((result == NULL) ^ (masterdict == NULL));
	if (masterdict != NULL) {
		/* The result comes from its keys. */
		assert(result == NULL);
		result = PyDict_Keys(masterdict);
		if (result == NULL)
			goto error;
	}

	assert(result);
	if (PyList_Sort(result) != 0)
		goto error;
	else
		goto normal_return;

  error:
	Py_XDECREF(result);
	result = NULL;
	/* fall through */
  normal_return:
  	Py_XDECREF(masterdict);
	return result;
}

/*
NoObject is usable as a non-NULL undefined value, used by the macro None.
There is (and should be!) no way to create other objects of this type,
so there is exactly one (which is indestructible, by the way).
(XXX This type and the type of NotImplemented below should be unified.)
*/

/* ARGSUSED */
static PyObject *
none_repr(PyObject *op)
{
	return PyString_FromString("None");
}

/* ARGUSED */
static void
none_dealloc(PyObject* ignore) 
{
	/* This should never get called, but we also don't want to SEGV if
	 * we accidently decref None out of existance.
	 */
	abort();
}


static PyTypeObject PyNone_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"NoneType",
	0,
	0,
	(destructor)none_dealloc,	     /*tp_dealloc*/ /*never called*/
	0,		/*tp_print*/
	0,		/*tp_getattr*/
	0,		/*tp_setattr*/
	0,		/*tp_compare*/
	(reprfunc)none_repr, /*tp_repr*/
	0,		/*tp_as_number*/
	0,		/*tp_as_sequence*/
	0,		/*tp_as_mapping*/
	0,		/*tp_hash */
};

PyObject _Py_NoneStruct = {
	PyObject_HEAD_INIT(&PyNone_Type)
};

/* NotImplemented is an object that can be used to signal that an
   operation is not implemented for the given type combination. */

static PyObject *
NotImplemented_repr(PyObject *op)
{
	return PyString_FromString("NotImplemented");
}

static PyTypeObject PyNotImplemented_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"NotImplementedType",
	0,
	0,
	(destructor)none_dealloc,	     /*tp_dealloc*/ /*never called*/
	0,		/*tp_print*/
	0,		/*tp_getattr*/
	0,		/*tp_setattr*/
	0,		/*tp_compare*/
	(reprfunc)NotImplemented_repr, /*tp_repr*/
	0,		/*tp_as_number*/
	0,		/*tp_as_sequence*/
	0,		/*tp_as_mapping*/
	0,		/*tp_hash */
};

PyObject _Py_NotImplementedStruct = {
	PyObject_HEAD_INIT(&PyNotImplemented_Type)
};

void
_Py_ReadyTypes(void)
{
	if (PyType_Ready(&PyType_Type) < 0)
		Py_FatalError("Can't initialize 'type'");

	if (PyType_Ready(&PyList_Type) < 0)
		Py_FatalError("Can't initialize 'list'");

	if (PyType_Ready(&PyNone_Type) < 0)
		Py_FatalError("Can't initialize type(None)");

	if (PyType_Ready(&PyNotImplemented_Type) < 0)
		Py_FatalError("Can't initialize type(NotImplemented)");
}


#ifdef Py_TRACE_REFS

static PyObject refchain = {&refchain, &refchain};

void
_Py_ResetReferences(void)
{
	refchain._ob_prev = refchain._ob_next = &refchain;
	_Py_RefTotal = 0;
}

void
_Py_NewReference(PyObject *op)
{
	_Py_RefTotal++;
	op->ob_refcnt = 1;
	op->_ob_next = refchain._ob_next;
	op->_ob_prev = &refchain;
	refchain._ob_next->_ob_prev = op;
	refchain._ob_next = op;
#ifdef COUNT_ALLOCS
	inc_count(op->ob_type);
#endif
}

void
_Py_ForgetReference(register PyObject *op)
{
#ifdef SLOW_UNREF_CHECK
        register PyObject *p;
#endif
	if (op->ob_refcnt < 0)
		Py_FatalError("UNREF negative refcnt");
	if (op == &refchain ||
	    op->_ob_prev->_ob_next != op || op->_ob_next->_ob_prev != op)
		Py_FatalError("UNREF invalid object");
#ifdef SLOW_UNREF_CHECK
	for (p = refchain._ob_next; p != &refchain; p = p->_ob_next) {
		if (p == op)
			break;
	}
	if (p == &refchain) /* Not found */
		Py_FatalError("UNREF unknown object");
#endif
	op->_ob_next->_ob_prev = op->_ob_prev;
	op->_ob_prev->_ob_next = op->_ob_next;
	op->_ob_next = op->_ob_prev = NULL;
#ifdef COUNT_ALLOCS
	op->ob_type->tp_frees++;
#endif
}

void
_Py_Dealloc(PyObject *op)
{
	destructor dealloc = op->ob_type->tp_dealloc;
	_Py_ForgetReference(op);
	(*dealloc)(op);
}

void
_Py_PrintReferences(FILE *fp)
{
	PyObject *op;
	fprintf(fp, "Remaining objects:\n");
	for (op = refchain._ob_next; op != &refchain; op = op->_ob_next) {
		fprintf(fp, "[%d] ", op->ob_refcnt);
		if (PyObject_Print(op, fp, 0) != 0)
			PyErr_Clear();
		putc('\n', fp);
	}
}

PyObject *
_Py_GetObjects(PyObject *self, PyObject *args)
{
	int i, n;
	PyObject *t = NULL;
	PyObject *res, *op;

	if (!PyArg_ParseTuple(args, "i|O", &n, &t))
		return NULL;
	op = refchain._ob_next;
	res = PyList_New(0);
	if (res == NULL)
		return NULL;
	for (i = 0; (n == 0 || i < n) && op != &refchain; i++) {
		while (op == self || op == args || op == res || op == t ||
		       (t != NULL && op->ob_type != (PyTypeObject *) t)) {
			op = op->_ob_next;
			if (op == &refchain)
				return res;
		}
		if (PyList_Append(res, op) < 0) {
			Py_DECREF(res);
			return NULL;
		}
		op = op->_ob_next;
	}
	return res;
}

#endif


/* Hack to force loading of cobject.o */
PyTypeObject *_Py_cobject_hack = &PyCObject_Type;


/* Hack to force loading of abstract.o */
int (*_Py_abstract_hack)(PyObject *) = &PyObject_Size;


/* Python's malloc wrappers (see pymem.h) */

void *
PyMem_Malloc(size_t nbytes)
{
#if _PyMem_EXTRA > 0
	if (nbytes == 0)
		nbytes = _PyMem_EXTRA;
#endif
	return PyMem_MALLOC(nbytes);
}

void *
PyMem_Realloc(void *p, size_t nbytes)
{
#if _PyMem_EXTRA > 0
	if (nbytes == 0)
		nbytes = _PyMem_EXTRA;
#endif
	return PyMem_REALLOC(p, nbytes);
}

void
PyMem_Free(void *p)
{
	PyMem_FREE(p);
}


/* Python's object malloc wrappers (see objimpl.h) */

void *
PyObject_Malloc(size_t nbytes)
{
	return PyObject_MALLOC(nbytes);
}

void *
PyObject_Realloc(void *p, size_t nbytes)
{
	return PyObject_REALLOC(p, nbytes);
}

void
PyObject_Free(void *p)
{
	PyObject_FREE(p);
}


/* These methods are used to control infinite recursion in repr, str, print,
   etc.  Container objects that may recursively contain themselves,
   e.g. builtin dictionaries and lists, should used Py_ReprEnter() and
   Py_ReprLeave() to avoid infinite recursion.

   Py_ReprEnter() returns 0 the first time it is called for a particular
   object and 1 every time thereafter.  It returns -1 if an exception
   occurred.  Py_ReprLeave() has no return value.

   See dictobject.c and listobject.c for examples of use.
*/

#define KEY "Py_Repr"

int
Py_ReprEnter(PyObject *obj)
{
	PyObject *dict;
	PyObject *list;
	int i;

	dict = PyThreadState_GetDict();
	if (dict == NULL)
		return -1;
	list = PyDict_GetItemString(dict, KEY);
	if (list == NULL) {
		list = PyList_New(0);
		if (list == NULL)
			return -1;
		if (PyDict_SetItemString(dict, KEY, list) < 0)
			return -1;
		Py_DECREF(list);
	}
	i = PyList_GET_SIZE(list);
	while (--i >= 0) {
		if (PyList_GET_ITEM(list, i) == obj)
			return 1;
	}
	PyList_Append(list, obj);
	return 0;
}

void
Py_ReprLeave(PyObject *obj)
{
	PyObject *dict;
	PyObject *list;
	int i;

	dict = PyThreadState_GetDict();
	if (dict == NULL)
		return;
	list = PyDict_GetItemString(dict, KEY);
	if (list == NULL || !PyList_Check(list))
		return;
	i = PyList_GET_SIZE(list);
	/* Count backwards because we always expect obj to be list[-1] */
	while (--i >= 0) {
		if (PyList_GET_ITEM(list, i) == obj) {
			PyList_SetSlice(list, i, i + 1, NULL);
			break;
		}
	}
}

/*
  trashcan
  CT 2k0130
  non-recursively destroy nested objects

  CT 2k0223
  everything is now done in a macro.

  CT 2k0305
  modified to use functions, after Tim Peter's suggestion.

  CT 2k0309
  modified to restore a possible error.

  CT 2k0325
  added better safe than sorry check for threadstate

  CT 2k0422
  complete rewrite. We now build a chain via ob_type
  and save the limited number of types in ob_refcnt.
  This is perfect since we don't need any memory.
  A patch for free-threading would need just a lock.
*/

#define Py_TRASHCAN_TUPLE       1
#define Py_TRASHCAN_LIST        2
#define Py_TRASHCAN_DICT        3
#define Py_TRASHCAN_FRAME       4
#define Py_TRASHCAN_TRACEBACK   5
/* extend here if other objects want protection */

int _PyTrash_delete_nesting = 0;

PyObject * _PyTrash_delete_later = NULL;

void
_PyTrash_deposit_object(PyObject *op)
{
	int typecode;

	if (PyTuple_Check(op))
		typecode = Py_TRASHCAN_TUPLE;
	else if (PyList_Check(op))
		typecode = Py_TRASHCAN_LIST;
	else if (PyDict_Check(op))
		typecode = Py_TRASHCAN_DICT;
	else if (PyFrame_Check(op))
		typecode = Py_TRASHCAN_FRAME;
	else if (PyTraceBack_Check(op))
		typecode = Py_TRASHCAN_TRACEBACK;
	else /* We have a bug here -- those are the only types in GC */ {
		Py_FatalError("Type not supported in GC -- internal bug");
		return; /* pacify compiler -- execution never here */
	}
	op->ob_refcnt = typecode;

	op->ob_type = (PyTypeObject*)_PyTrash_delete_later;
	_PyTrash_delete_later = op;
}

void
_PyTrash_destroy_chain(void)
{
	while (_PyTrash_delete_later) {
		PyObject *shredder = _PyTrash_delete_later;
		_PyTrash_delete_later = (PyObject*) shredder->ob_type;

		switch (shredder->ob_refcnt) {
		case Py_TRASHCAN_TUPLE:
			shredder->ob_type = &PyTuple_Type;
			break;
		case Py_TRASHCAN_LIST:
			shredder->ob_type = &PyList_Type;
			break;
		case Py_TRASHCAN_DICT:
			shredder->ob_type = &PyDict_Type;
			break;
		case Py_TRASHCAN_FRAME:
			shredder->ob_type = &PyFrame_Type;
			break;
		case Py_TRASHCAN_TRACEBACK:
			shredder->ob_type = &PyTraceBack_Type;
			break;
		}
		_Py_NewReference(shredder);

		++_PyTrash_delete_nesting;
		Py_DECREF(shredder);
		--_PyTrash_delete_nesting;
	}
}

#ifdef WITH_PYMALLOC
#include "obmalloc.c"
#endif
