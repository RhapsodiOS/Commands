# -*-mode: python; fill-column: 75; tab-width: 8; coding: iso-latin-1-unix -*-
#
#	$Id: DirList.py,v 1.1.1.1 2002/02/05 23:23:10 zarzycki Exp $
#
# Tix Demostration Program
#
# This sample program is structured in such a way so that it can be
# executed from the Tix demo program "widget": it must have a
# procedure called "RunSample". It should also have the "if" statment
# at the end of this file so that it can be run as a standalone
# program using tixwish.

# This file demonstrates the use of the tixDirList widget -- you can
# use it for the user to select a directory. For example, an installation
# program can use the tixDirList widget to ask the user to select the
# installation directory for an application.
#

import Tix, os, copy
from Tkconstants import *

TCL_DONT_WAIT		= 1<<1
TCL_WINDOW_EVENTS	= 1<<2
TCL_FILE_EVENTS		= 1<<3
TCL_TIMER_EVENTS	= 1<<4
TCL_IDLE_EVENTS		= 1<<5
TCL_ALL_EVENTS		= 0

def RunSample (root):
    global dirlist
    dirlist = DemoDirList(root)
    dirlist.mainloop()
    dirlist.destroy()
    
class DemoDirList:
    def __init__(self, w):
        self.root = w
        self.exit = -1
        
        z = w.winfo_toplevel()
        z.wm_title('Tix.DirList Widget Demo')
        
        # Create the tixDirList and the tixLabelEntry widgets on the on the top
        # of the dialog box

        # bg = root.tk.eval('tix option get bg')
        # adding bg=bg crashes Windows pythonw tk8.3.3 Python 2.1.0

        top = Tix.Frame( w, relief=RAISED, bd=1)

        # Create the DirList widget. By default it will show the current
        # directory
        #
        #
        top.dir = Tix.DirList(top)
        top.dir.hlist['width'] = 40
        
        # When the user presses the ".." button, the selected directory
        # is "transferred" into the entry widget
        #
        top.btn = Tix.Button(top, text = "  >>  ", pady = 0)

        # We use a LabelEntry to hold the installation directory. The user
        # can choose from the DirList widget, or he can type in the directory 
        # manually
        #
        top.ent = Tix.LabelEntry(top, label="Installation Directory:",
                                  labelside = 'top',
                                  options = '''
                                  entry.width 40
                                  label.anchor w
                                  ''')
        
        font = self.root.tk.eval('tix option get fixed_font')
        # font = self.root.master.tix_option_get('fixed_font')
        top.ent.entry['font'] = font

        self.dlist_dir = copy.copy(os.curdir)
        # This should work setting the entry's textvariable
        top.ent.entry['textvariable'] = self.dlist_dir
        top.btn['command'] = lambda dir=top.dir, ent=top.ent, self=self: \
                             self.copy_name(dir,ent)

        # top.ent.entry.insert(0,'tix'+`self`)
        top.ent.entry.bind('<Return>', lambda self=self: self.okcmd () )

        top.pack( expand='yes', fill='both', side=TOP)
        top.dir.pack( expand=1, fill=BOTH, padx=4, pady=4, side=LEFT)
        top.btn.pack( anchor='s', padx=4, pady=4, side=LEFT)
        top.ent.pack( expand=1, fill=X, anchor='s', padx=4, pady=4, side=LEFT)

        # Use a ButtonBox to hold the buttons.
        #
        box = Tix.ButtonBox (w, orientation='horizontal')
        box.add ('ok', text='Ok', underline=0, width=6,
                     command = lambda self=self: self.okcmd () )
        box.add ('cancel', text='Cancel', underline=0, width=6,
                     command = lambda self=self: self.quitcmd () )

        box.pack( anchor='s', fill='x', side=BOTTOM)
        z.wm_protocol("WM_DELETE_WINDOW", lambda self=self: self.quitcmd())

    def copy_name (self, dir, ent):
        # This should work as it is the entry's textvariable
        self.dlist_dir = dir.cget('value')
        # but it isn't so I'll do it manually
        ent.entry.delete(0,'end')
        ent.entry.insert(0, self.dlist_dir)

    def okcmd (self):
        # tixDemo:Status "You have selected the directory" + $self.dlist_dir

        self.quitcmd()

    def quitcmd (self):
        # self.root.destroy()
        self.exit = 0

    def mainloop(self):
        while self.exit < 0:
            self.root.tk.dooneevent(TCL_ALL_EVENTS)
        # self.root.tk.dooneevent(TCL_DONT_WAIT)

    def destroy (self):
        self.root.destroy()

# This "if" statement makes it possible to run this script file inside or
# outside of the main demo program "widget".
#
if __name__== '__main__' :
    import tkMessageBox, traceback

    try:
        root=Tix.Tk()
        RunSample(root)
    except:
        t, v, tb = sys.exc_info()
        text = "Error running the demo script:\n"
        for line in traceback.format_exception(t,v,tb):
            text = text + line + '\n'
            d = tkMessageBox.showerror ( 'Tix Demo Error', text)

