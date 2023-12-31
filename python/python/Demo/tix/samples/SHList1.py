# -*-mode: python; fill-column: 75; tab-width: 8; coding: iso-latin-1-unix -*-
# 
# $Id: SHList1.py,v 1.1.1.1 2002/02/05 23:23:10 zarzycki Exp $
#
# Tix Demostration Program
#
# This sample program is structured in such a way so that it can be
# executed from the Tix demo program "tixwidgets": it must have a
# procedure called "RunSample". It should also have the "if" statment
# at the end of this file so that it can be run as a standalone
# program using tixwish.

# This file demonstrates the use of the tixScrolledHList widget.
#

import Tix

def RunSample (w) :

    # We create the frame and the ScrolledHList widget
    # at the top of the dialog box
    #
    top = Tix.Frame( w, relief=Tix.RAISED, bd=1)

    # Put a simple hierachy into the HList (two levels). Use colors and
    # separator widgets (frames) to make the list look fancy
    #
    top.a  = Tix.ScrolledHList(top)
    top.a.pack( expand=1, fill=Tix.BOTH, padx=10, pady=10, side=Tix.TOP)


    # This is our little relational database
    #
    bosses = [
	('jeff',  'Jeff Waxman'),
	('john',  'John Lee'),
	('peter', 'Peter Kenson')
    ]

    employees = [
	('alex',  'john',  'Alex Kellman'),
	('alan',  'john',  'Alan Adams'),
	('andy',  'peter', 'Andreas Crawford'),
	('doug',  'jeff',  'Douglas Bloom'),
	('jon',   'peter', 'Jon Baraki'),
	('chris', 'jeff',  'Chris Geoffrey'),
	('chuck', 'jeff',  'Chuck McLean')
    ]

    hlist=top.a.hlist

    # Let configure the appearance of the HList subwidget 
    #
    hlist.config( separator='.', width=25, drawbranch=0, indent=10)

    count=0
    for boss,name in bosses :
	if count :
	    f=Tix.Frame(hlist, name='sep%d' % count, height=2, width=150,
		bd=2, relief=Tix.SUNKEN )

	    hlist.add_child( itemtype=Tix.WINDOW, 
		window=f, state=Tix.DISABLED )
	
	hlist.add(boss, itemtype=Tix.TEXT, text=name)
	count = count+1
    

    for person,boss,name in employees :
	# '.' is the separator character we chose above
	#
	key= boss    + '.'     + person
	#    ^^^^                ^^^^^^
	#    parent entryPath /  child's name

	hlist.add( key, text=name )

	# [Hint] Make sure the keys (e.g. 'boss.person') you choose
	#	 are unique names. If you cannot be sure of this (because of
	#	 the structure of your database, e.g.) you can use the
	#	 "add_child" command instead:
	#
	#  hlist.addchild( boss,  text=name)
	#                  ^^^^
	#                  parent entryPath 


    # Use a ButtonBox to hold the buttons.
    #
    box= Tix.ButtonBox(top, orientation=Tix.HORIZONTAL )
    box.add( 'ok',  text='Ok', underline=0,  width=6,
	command = lambda w=w:  w.quit() )

    box.add( 'cancel', text='Cancel', underline=0, width=6,
	command = lambda w=w:  w.quit() )

    box.pack( side=Tix.BOTTOM, fill=Tix.X)
    top.pack( side=Tix.TOP,    fill=Tix.BOTH, expand=1 )


# This "if" statement makes it possible to run this script file inside or
# outside of the main demo program "widget".
#
if __name__== '__main__' :
    root=Tix.Tk()
    RunSample(root)
    root.mainloop()
    root.destroy()
