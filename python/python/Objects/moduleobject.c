
/* Module object implementation */

#include "Python.h"
#include "structmember.h"

typedef struct {
	PyObject_HEAD
	PyObject *md_dict;
} PyModuleObject;

static PyMemberDef module_members[] = {
	{"__dict__", T_OBJECT, offsetof(PyModuleObject, md_dict), READONLY},
	{0}
};

PyObject *
PyModule_New(char *name)
{
	PyModuleObject *m;
	PyObject *nameobj;
	m = PyObject_GC_New(PyModuleObject, &PyModule_Type);
	if (m == NULL)
		return NULL;
	nameobj = PyString_FromString(name);
	m->md_dict = PyDict_New();
	if (m->md_dict == NULL || nameobj == NULL)
		goto fail;
	if (PyDict_SetItemString(m->md_dict, "__name__", nameobj) != 0)
		goto fail;
	if (PyDict_SetItemString(m->md_dict, "__doc__", Py_None) != 0)
		goto fail;
	Py_DECREF(nameobj);
	PyObject_GC_Track(m);
	return (PyObject *)m;

 fail:
	Py_XDECREF(nameobj);
	Py_DECREF(m);
	return NULL;
}

PyObject *
PyModule_GetDict(PyObject *m)
{
	if (!PyModule_Check(m)) {
		PyErr_BadInternalCall();
		return NULL;
	}
	return ((PyModuleObject *)m) -> md_dict;
}

char *
PyModule_GetName(PyObject *m)
{
	PyObject *nameobj;
	if (!PyModule_Check(m)) {
		PyErr_BadArgument();
		return NULL;
	}
	nameobj = PyDict_GetItemString(((PyModuleObject *)m)->md_dict,
				       "__name__");
	if (nameobj == NULL || !PyString_Check(nameobj)) {
		PyErr_SetString(PyExc_SystemError, "nameless module");
		return NULL;
	}
	return PyString_AsString(nameobj);
}

char *
PyModule_GetFilename(PyObject *m)
{
	PyObject *fileobj;
	if (!PyModule_Check(m)) {
		PyErr_BadArgument();
		return NULL;
	}
	fileobj = PyDict_GetItemString(((PyModuleObject *)m)->md_dict,
				       "__file__");
	if (fileobj == NULL || !PyString_Check(fileobj)) {
		PyErr_SetString(PyExc_SystemError, "module filename missing");
		return NULL;
	}
	return PyString_AsString(fileobj);
}

void
_PyModule_Clear(PyObject *m)
{
	/* To make the execution order of destructors for global
	   objects a bit more predictable, we first zap all objects
	   whose name starts with a single underscore, before we clear
	   the entire dictionary.  We zap them by replacing them with
	   None, rather than deleting them from the dictionary, to
	   avoid rehashing the dictionary (to some extent). */

	int pos;
	PyObject *key, *value;
	PyObject *d;

	d = ((PyModuleObject *)m)->md_dict;

	/* First, clear only names starting with a single underscore */
	pos = 0;
	while (PyDict_Next(d, &pos, &key, &value)) {
		if (value != Py_None && PyString_Check(key)) {
			char *s = PyString_AsString(key);
			if (s[0] == '_' && s[1] != '_') {
				if (Py_VerboseFlag > 1)
				    PySys_WriteStderr("#   clear[1] %s\n", s);
				PyDict_SetItem(d, key, Py_None);
			}
		}
	}

	/* Next, clear all names except for __builtins__ */
	pos = 0;
	while (PyDict_Next(d, &pos, &key, &value)) {
		if (value != Py_None && PyString_Check(key)) {
			char *s = PyString_AsString(key);
			if (s[0] != '_' || strcmp(s, "__builtins__") != 0) {
				if (Py_VerboseFlag > 1)
				    PySys_WriteStderr("#   clear[2] %s\n", s);
				PyDict_SetItem(d, key, Py_None);
			}
		}
	}

	/* Note: we leave __builtins__ in place, so that destructors
	   of non-global objects defined in this module can still use
	   builtins, in particularly 'None'. */

}

/* Methods */

static int
module_init(PyModuleObject *m, PyObject *args, PyObject *kw)
{
	m->md_dict = PyDict_New();
	if (m->md_dict == NULL)
		return -1;
	return 0;
}

static void
module_dealloc(PyModuleObject *m)
{
	PyObject_GC_UnTrack(m);
	if (m->md_dict != NULL) {
		_PyModule_Clear((PyObject *)m);
		Py_DECREF(m->md_dict);
	}
	m->ob_type->tp_free((PyObject *)m);
}

static PyObject *
module_repr(PyModuleObject *m)
{
	char *name;
	char *filename;

	name = PyModule_GetName((PyObject *)m);
	if (name == NULL) {
		PyErr_Clear();
		name = "?";
	}
	filename = PyModule_GetFilename((PyObject *)m);
	if (filename == NULL) {
		PyErr_Clear();
		return PyString_FromFormat("<module '%s' (built-in)>", name);
	}
	return PyString_FromFormat("<module '%s' from '%s'>", name, filename);
}

/* We only need a traverse function, no clear function: If the module
   is in a cycle, md_dict will be cleared as well, which will break
   the cycle. */
static int
module_traverse(PyModuleObject *m, visitproc visit, void *arg)
{
	if (m->md_dict != NULL)
		return visit(m->md_dict, arg);
	return 0;
}

PyTypeObject PyModule_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,					/* ob_size */
	"module",				/* tp_name */
	sizeof(PyModuleObject),			/* tp_size */
	0,					/* tp_itemsize */
	(destructor)module_dealloc,		/* tp_dealloc */
	0,					/* tp_print */
	0,					/* tp_getattr */
	0,					/* tp_setattr */
	0,					/* tp_compare */
	(reprfunc)module_repr,			/* tp_repr */
	0,					/* tp_as_number */
	0,					/* tp_as_sequence */
	0,					/* tp_as_mapping */
	0,					/* tp_hash */
	0,					/* tp_call */
	0,					/* tp_str */
	PyObject_GenericGetAttr,		/* tp_getattro */
	PyObject_GenericSetAttr,		/* tp_setattro */
	0,					/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC |
		Py_TPFLAGS_BASETYPE,		/* tp_flags */
	0,					/* tp_doc */
	(traverseproc)module_traverse,		/* tp_traverse */
	0,					/* tp_clear */
	0,					/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	0,					/* tp_methods */
	module_members,				/* tp_members */
	0,					/* tp_getset */
	0,					/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	offsetof(PyModuleObject, md_dict),	/* tp_dictoffset */
	(initproc)module_init,			/* tp_init */
	PyType_GenericAlloc,			/* tp_alloc */
	PyType_GenericNew,			/* tp_new */
	_PyObject_GC_Del,			/* tp_free */
};
