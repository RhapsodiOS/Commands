#!/usr/local/bin/python
# 
# $Id: Control.py,v 1.1.1.1 2002/02/05 23:23:10 zarzycki Exp $
#
# Tix Demostration Program
#
# This sample program is structured in such a way so that it can be
# executed from the Tix demo program "tixwidgets": it must have a
# procedure called "RunSample". It should also have the "if" statment
# at the end of this file so that it can be run as a standalone
# program.

# This file demonstrates the use of the tixControl widget -- it is an
# entry widget with up/down arrow buttons. You can use the arrow buttons
# to adjust the value inside the entry widget.
#
# This example program uses three Control widgets. One lets you select
# integer values; one lets you select floating point values and the last
# one lets you select a few names.
#
import Tix

def RunSample(w):
    global demo_maker, demo_thrust, demo_num_engines

    demo_maker = Tix.StringVar()
    demo_thrust = Tix.DoubleVar()
    demo_num_engines = Tix.IntVar()
    demo_maker.set('P&W')
    demo_thrust.set(20000.0)
    demo_num_engines.set(2)

    top = Tix.Frame(w, bd=1, relief=Tix.RAISED)

    # $w.top.a allows only integer values
    #
    # [Hint] The -options switch sets the options of the subwidgets.
    # [Hint] We set the label.width subwidget option of the Controls to 
    #        be 16 so that their labels appear to be aligned.
    #
    a = Tix.Control(top, label='Number of Engines: ', integer=1,
		    variable=demo_num_engines, min=1, max=4,
		    options='entry.width 10 label.width 20 label.anchor e')
    
    b = Tix.Control(top, label='Thrust: ', integer=0,
		    min='10000.0', max='60000.0', step=500,
		    variable=demo_thrust,
		    options='entry.width 10 label.width 20 label.anchor e')

    c = Tix.Control(top, label='Engine Maker: ', value='P&W',
		    variable=demo_maker,
		    options='entry.width 10 label.width 20 label.anchor e')

    # We can't define these in the init because the widget 'c' doesn't
    # exist yet and we need to reference it
    c['incrcmd'] = lambda w=c: adjust_maker(w, 1)
    c['decrcmd'] = lambda w=c: adjust_maker(w, -1)
    c['validatecmd'] = lambda w=c: validate_maker(w)

    a.pack(side=Tix.TOP, anchor=Tix.W)
    b.pack(side=Tix.TOP, anchor=Tix.W)
    c.pack(side=Tix.TOP, anchor=Tix.W)

    box = Tix.ButtonBox(w, orientation=Tix.HORIZONTAL)
    box.add('ok', text='Ok', underline=0, width=6,
	    command=lambda w=w: ok_command(w))
    box.add('cancel', text='Cancel', underline=0, width=6,
	    command=lambda w=w: w.destroy())
    box.pack(side=Tix.BOTTOM, fill=Tix.X)
    top.pack(side=Tix.TOP, fill=Tix.BOTH, expand=1)

maker_list = ['P&W', 'GE', 'Rolls Royce']

def adjust_maker(w, inc):
    i = maker_list.index(demo_maker.get())
    i = i + inc
    if i >= len(maker_list):
	i = 0
    elif i < 0:
	i = len(maker_list) - 1

    # In Tcl/Tix we should return the string maker_list[i]. We can't
    # do that in Tkinter so we set the global variable. (This works).
    demo_maker.set(maker_list[i])

def validate_maker(w):
    try:
	i = maker_list.index(demo_maker.get())
    except ValueError:
	# Works here though. Why ? Beats me.
	return maker_list[0]
    # Works here though. Why ? Beats me.
    return maker_list[i]

def ok_command(w):
    print "Selected", demo_num_engines.get(), "of", demo_maker.get(), " engines each of thrust", demo_thrust.get()
    w.destroy()

if __name__ == '__main__':
    root = Tix.Tk()
    RunSample(root)
    root.mainloop()
