# This script generates a Python interface for an Apple Macintosh Manager.
# It uses the "bgen" package to generate C code.
# The function specifications are generated by scanning the mamager's header file,
# using the "scantools" package (customized for this particular manager).

#error missing SetActionFilter

import string

# Declarations that change for each manager
MODNAME = '_CG'				# The name of the module

# The following is *usually* unchanged but may still require tuning
MODPREFIX = 'CG'			# The prefix for module-wide routines
INPUTFILE = string.lower(MODPREFIX) + 'gen.py' # The file generated by the scanner
OUTPUTFILE = MODNAME + "module.c"	# The file generated by this program

from macsupport import *


# Create the type objects

includestuff = includestuff + """
#ifdef WITHOUT_FRAMEWORKS
#include <Quickdraw.h>
#include <CGContext.h>
#else
#include <ApplicationServices/ApplicationServices.h>
#endif

#if !TARGET_API_MAC_OSX
	/* This code is adapted from the CallMachOFramework demo at:
       http://developer.apple.com/samplecode/Sample_Code/Runtime_Architecture/CallMachOFramework.htm
       It allows us to call Mach-O functions from CFM apps. */

	#include <Folders.h>
	#include "CFMLateImport.h"

	static OSStatus LoadFrameworkBundle(CFStringRef framework, CFBundleRef *bundlePtr)
		// This routine finds a the named framework and creates a CFBundle 
		// object for it.  It looks for the framework in the frameworks folder, 
		// as defined by the Folder Manager.  Currently this is 
		// "/System/Library/Frameworks", but we recommend that you avoid hard coded 
		// paths to ensure future compatibility.
		//
		// You might think that you could use CFBundleGetBundleWithIdentifier but 
		// that only finds bundles that are already loaded into your context. 
		// That would work in the case of the System framework but it wouldn't 
		// work if you're using some other, less-obvious, framework.
	{
		OSStatus 	err;
		FSRef 		frameworksFolderRef;
		CFURLRef	baseURL;
		CFURLRef	bundleURL;
		
		*bundlePtr = nil;
		
		baseURL = nil;
		bundleURL = nil;
		
		// Find the frameworks folder and create a URL for it.
		
		err = FSFindFolder(kOnAppropriateDisk, kFrameworksFolderType, true, &frameworksFolderRef);
		if (err == noErr) {
			baseURL = CFURLCreateFromFSRef(kCFAllocatorSystemDefault, &frameworksFolderRef);
			if (baseURL == nil) {
				err = coreFoundationUnknownErr;
			}
		}
		
		// Append the name of the framework to the URL.
		
		if (err == noErr) {
			bundleURL = CFURLCreateCopyAppendingPathComponent(kCFAllocatorSystemDefault, baseURL, framework, false);
			if (bundleURL == nil) {
				err = coreFoundationUnknownErr;
			}
		}
		
		// Create a bundle based on that URL and load the bundle into memory.
		// We never unload the bundle, which is reasonable in this case because 
		// the sample assumes that you'll be calling functions from this 
		// framework throughout the life of your application.
		
		if (err == noErr) {
			*bundlePtr = CFBundleCreate(kCFAllocatorSystemDefault, bundleURL);
			if (*bundlePtr == nil) {
				err = coreFoundationUnknownErr;
			}
		}
		if (err == noErr) {
		    if ( ! CFBundleLoadExecutable( *bundlePtr ) ) {
				err = coreFoundationUnknownErr;
		    }
		}

		// Clean up.
		
		if (err != noErr && *bundlePtr != nil) {
			CFRelease(*bundlePtr);
			*bundlePtr = nil;
		}
		if (bundleURL != nil) {
			CFRelease(bundleURL);
		}	
		if (baseURL != nil) {
			CFRelease(baseURL);
		}	
		
		return err;
	}



	// The CFMLateImport approach requires that you define a fragment 
	// initialisation routine that latches the fragment's connection 
	// ID and locator.  If your code already has a fragment initialiser 
	// you will have to integrate the following into it.

	static CFragConnectionID 			gFragToFixConnID;
	static FSSpec 						gFragToFixFile;
	static CFragSystem7DiskFlatLocator 	gFragToFixLocator;

	extern OSErr FragmentInit(const CFragInitBlock *initBlock);
	extern OSErr FragmentInit(const CFragInitBlock *initBlock)
	{
		__initialize(initBlock); /* call the "original" initializer */
		gFragToFixConnID	= (CFragConnectionID) initBlock->closureID;
		gFragToFixFile 		= *(initBlock->fragLocator.u.onDisk.fileSpec);
		gFragToFixLocator 	= initBlock->fragLocator.u.onDisk;
		gFragToFixLocator.fileSpec = &gFragToFixFile;
		
		return noErr;
	}

#endif

extern int GrafObj_Convert(PyObject *, GrafPtr *);

/*
** Manual converters
*/

PyObject *CGPoint_New(CGPoint *itself)
{

	return Py_BuildValue("(ff)",
			itself->x,
			itself->y);
}

int
CGPoint_Convert(PyObject *v, CGPoint *p_itself)
{
	if( !PyArg_Parse(v, "(ff)",
			&p_itself->x,
			&p_itself->y) )
		return 0;
	return 1;
}

PyObject *CGRect_New(CGRect *itself)
{

	return Py_BuildValue("(ffff)",
			itself->origin.x,
			itself->origin.y,
			itself->size.width,
			itself->size.height);
}

int
CGRect_Convert(PyObject *v, CGRect *p_itself)
{
	if( !PyArg_Parse(v, "(ffff)",
			&p_itself->origin.x,
			&p_itself->origin.y,
			&p_itself->size.width,
			&p_itself->size.height) )
		return 0;
	return 1;
}

PyObject *CGAffineTransform_New(CGAffineTransform *itself)
{

	return Py_BuildValue("(ffffff)",
			itself->a,
			itself->b,
			itself->c,
			itself->d,
			itself->tx,
			itself->ty);
}

int
CGAffineTransform_Convert(PyObject *v, CGAffineTransform *p_itself)
{
	if( !PyArg_Parse(v, "(ffffff)",
			&p_itself->a,
			&p_itself->b,
			&p_itself->c,
			&p_itself->d,
			&p_itself->tx,
			&p_itself->ty) )
		return 0;
	return 1;
}
"""

initstuff = initstuff + """
#if !TARGET_API_MAC_OSX
CFBundleRef sysBundle;
OSStatus err;

if (&LoadFrameworkBundle == NULL) {
	PyErr_SetString(PyExc_ImportError, "CoreCraphics not supported");
	return;
}
err = LoadFrameworkBundle(CFSTR("ApplicationServices.framework"), &sysBundle);
if (err == noErr)
	err = CFMLateImportBundle(&gFragToFixLocator, gFragToFixConnID, FragmentInit, "\pCGStubLib", sysBundle);
if (err != noErr) {
	PyErr_SetString(PyExc_ImportError, "CoreCraphics not supported");
	return;
};
#endif  /* !TARGET_API_MAC_OSX */
"""

class MyOpaqueByValueType(OpaqueByValueType):
	"""Sort of a mix between OpaqueByValueType and OpaqueType."""
	def mkvalueArgs(self, name):
		return "%s, &%s" % (self.new, name)

CGPoint = MyOpaqueByValueType('CGPoint', 'CGPoint')
CGRect = MyOpaqueByValueType('CGRect', 'CGRect')
CGAffineTransform = MyOpaqueByValueType('CGAffineTransform', 'CGAffineTransform')

char_ptr = Type("char *", "s")

CGTextEncoding = int
CGLineCap = int
CGLineJoin = int
CGTextDrawingMode = int
CGPathDrawingMode = int

# The real objects
CGContextRef = OpaqueByValueType("CGContextRef", "CGContextRefObj")


class MyObjectDefinition(GlobalObjectDefinition):
	def outputStructMembers(self):
		ObjectDefinition.outputStructMembers(self)
	def outputCleanupStructMembers(self):
		Output("CGContextRelease(self->ob_itself);")


# Create the generator groups and link them
module = MacModule(MODNAME, MODPREFIX, includestuff, finalstuff, initstuff)

CGContextRef_object = MyObjectDefinition('CGContextRef', 'CGContextRefObj', 'CGContextRef')


# ADD object here

module.addobject(CGContextRef_object)



Function = FunctionGenerator
Method = MethodGenerator

CGContextRef_methods = []

# ADD _methods initializer here
execfile(INPUTFILE)

CreateCGContextForPort_body = """\
GrafPtr port;
CGContextRef ctx;
OSStatus _err;

if (!PyArg_ParseTuple(_args, "O&", GrafObj_Convert, &port))
	return NULL;

_err = CreateCGContextForPort(port, &ctx);
if (_err != noErr)
	if (_err != noErr) return PyMac_Error(_err);
_res = Py_BuildValue("O&", CGContextRefObj_New, ctx);
return _res;
"""

f = ManualGenerator("CreateCGContextForPort", CreateCGContextForPort_body);
f.docstring = lambda: "(CGrafPtr) -> CGContextRef"
module.add(f)


# ADD add forloop here
for f in CGContextRef_methods:
	CGContextRef_object.add(f)

# generate output (open the output file as late as possible)
SetOutputFileName(OUTPUTFILE)
module.generate()

