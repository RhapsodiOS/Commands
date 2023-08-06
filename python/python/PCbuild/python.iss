; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

; This is the whole ball of wax for an Inno installer for Python.
; To use, download Inno Setup from http://www.jrsoftware.org/isdl.htm/,
; install it, and double-click on this file.  That launches the Inno
; script compiler.  The GUI is extemely simple, and has only one button
; you may not recognize instantly:  click it.  You're done.  It builds
; the installer into PCBuild/Python-2.2a1.exe.  Size and speed of the
; installer are competitive with the Wise installer; Inno uninstall
; seems much quicker than Wise (but also feebler, and the uninstall
; log is in some un(human)readable binary format).
;
; What's Done
; -----------
; All the usual Windows Python files are installed by this now.
; All the usual Windows Python Start menu entries are created and
; work fine.
; .py, .pyw, .pyc and .pyo extensions are registered.
;     PROBLEM:  Inno uninstall does not restore their previous registry
;               associations (if any).  Wise did.  This will make life
;               difficult for alpha (etc) testers.
; The Python install is fully functional for "typical" uses.
;
; What's Not Done
; ---------------
; None of "Mark Hammond's" registry entries are written.
; No installation of files is done into the system dir:
;     The MS DLLs aren't handled at all by this yet.
;     Python22.dll is unpacked into the main Python dir.
;
; Inno can't do different things on NT/2000 depending on whether the user
; has Admin privileges, so I don't know how to "solve" either of those,
; short of building two installers (one *requiring* Admin privs, the
; other not doing anything that needs Admin privs).
;
; Inno has no concept of variables, so lots of lines in this file need
; to be fiddled by hand across releases.  Simplest way out:  stick this
; file in a giant triple-quoted r-string (note that backslashes are
; required all over the place here -- forward slashes DON'T WORK in
; Inno), and use %(yadda)s string interpolation to do substitutions; i.e.,
; write a very simple Python program to *produce* this script.

[Setup]
; Note:  we *want* the version number to show up everywhere.
; Which of these controls App Path???
AppName=Python 2.2 alpha 1
AppVerName=Python 2.2 alpha 1
AppId=Python 2.2
AppVersion=2.2a1
AppCopyright=Copyright � 2001 Python Software Foundation

; Default install dir; value of {app} later (unless user overrides).
; {sd} = system root drive, probably "C:".
DefaultDirName={sd}\Python22

; Start menu folder name; value of {group} later (unless user overrides).
DefaultGroupName=Python 2.2

; Point SourceDir to one above PCBuild = src.
; YAY!  That actually worked:  means this script can run unchanged from anyone's
; CVS tree, no matter what they called the top-level directories.  Wise
; required hardcoded absolute paths all over the place.
SourceDir=..
OutputDir=PCBuild
OutputBaseFilename=Python-2.2a1

AppPublisher=PythonLabs at Digital Creations
AppPublisherURL=http://www.python.org
AppSupportURL=http://www.python.org
AppUpdatesURL=http://www.python.org

AlwaysCreateUninstallIcon=yes
ChangesAssociations=yes
UninstallLogMode=new

; The fewer screens the better; leave these commented.
;LicenseFile=LICENSE
;InfoBeforeFile=Misc\NEWS

; uncomment the following line if you want your installation to run on NT 3.51 too.
; MinVersion=4,3.51

[Types]
Name: normal; Description: "Select desired components"; Flags: iscustom

[Components]
Name: main;  Description: "Python interpreter, library and Tk"; Types: normal
Name: docs;  Description: "Python documentation (HTML)";        Types: normal
Name: tools; Description: "Python utility scripts (Tools\)";    Types: normal
Name: test;  Description: "Python test suite (Lib\test\)";      Types: normal

[Tasks]
Name: startmenu; Description: "Create Start menu shortcuts"; Components: main docs tools
Name: extensions; Description: "Register file extensions (.py, .pyw, .pyc, .pyo)"; Components: main

[Files]
; Caution:  Using forward slashes instead screws up in amazing ways.
; Unknown:  By the time Components (and other attrs) are added to these lines, they're
; going to get awfully long.  But don't see a way to continue logical lines across
; physical lines.

Source: LICENSE;   DestDir: "{app}"; DestName: LICENSE.txt; CopyMode: alwaysoverwrite
Source: README;    DestDir: "{app}"; DestName: README.txt;  CopyMode: alwaysoverwrite
Source: Misc\News; DestDir: "{app}"; DestName: NEWS.txt;    CopyMode: alwaysoverwrite
Source: PC\*.ico;  DestDir: "{app}";                        CopyMode: alwaysoverwrite; Components: main


Source: PCbuild\python.exe;   DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\pythonw.exe;  DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\w9xpopen.exe; DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\python22.dll; DestDir: "{app}"; CopyMode: alwaysoverwrite; Components: main

Source: ..\tcl\bin\tcl83.dll; DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: ..\tcl\bin\tk83.dll;  DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: ..\tcl\lib\*.*;       DestDir: "{app}\tcl";  CopyMode: alwaysoverwrite; Components: main; Flags: recursesubdirs

Source: PCbuild\_socket.pyd;     DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\_socket.lib;     DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\_sre.pyd;        DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\_sre.lib;        DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\_symtable.pyd;   DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\_symtable.lib;   DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\_testcapi.pyd;   DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\_testcapi.lib;   DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\_tkinter.pyd;    DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\_tkinter.lib;    DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\bsddb.pyd;       DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\bsddb.lib;       DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\mmap.pyd;        DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\mmap.lib;        DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\parser.pyd;      DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\parser.lib;      DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\pyexpat.pyd;     DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\pyexpat.lib;     DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\select.pyd;      DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\select.lib;      DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\unicodedata.pyd; DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\unicodedata.lib; DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\_winreg.pyd;     DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\_winreg.lib;     DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\winsound.pyd;    DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\winsound.lib;    DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\zlib.pyd;        DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main
Source: PCbuild\zlib.lib;        DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: PCbuild\python22.lib;    DestDir: "{app}\libs"; CopyMode: alwaysoverwrite; Components: main

Source: ..\expat\Libs\expat.dll; DestDir: "{app}\DLLs"; CopyMode: alwaysoverwrite; Components: main

Source: Lib\*.py; DestDir: "{app}\Lib"; CopyMode: alwaysoverwrite; Components: main
Source: Lib\lib-tk\*.py; DestDir: "{app}\Lib\lib-tk"; CopyMode: alwaysoverwrite; Components: main

Source: Lib\encodings\*.py; DestDir: "{app}\Lib\encodings"; CopyMode: alwaysoverwrite; Components: main
Source: Lib\distutils\*.py; DestDir: "{app}\Lib\distutils"; CopyMode: alwaysoverwrite; Components: main
Source: Lib\xml\*.py; DestDir: "{app}\Lib\xml"; CopyMode: alwaysoverwrite; Flags: recursesubdirs; Components: main
Source: Lib\lib-old\*.py;  DestDir: "{app}\Lib\lib-old"; CopyMode: alwaysoverwrite; Components: main
Source: Lib\site-packages\README; DestDir: "{app}\Lib\site-packages"; DestName: README.txt; CopyMode: alwaysoverwrite; Components: main
Source: Include\*.h; DestDir: "{app}\include"; CopyMode: alwaysoverwrite; Components: main
Source: PC\pyconfig.h; DestDir: "{app}\include"; CopyMode: alwaysoverwrite; Components: main

Source: Tools\scripts\*.py;  DestDir: "{app}\Tools\Scripts";  CopyMode: alwaysoverwrite; Components: tools
Source: Tools\scripts\*.pyw; DestDir: "{app}\Tools\Scripts";  CopyMode: alwaysoverwrite; Components: tools
Source: Tools\scripts\*.doc; DestDir: "{app}\Tools\Scripts";  CopyMode: alwaysoverwrite; Components: tools
Source: Tools\scripts\README; DestDir: "{app}\Tools\Scripts"; DestName: README.txt;      CopyMode: alwaysoverwrite; Components: tools

Source: Tools\webchecker\*.py;   DestDir: "{app}\Tools\webchecker"; CopyMode: alwaysoverwrite; Components: tools
Source: Tools\webchecker\README; DestDir: "{app}\Tools\webchecker"; DestName: README.txt;      CopyMode: alwaysoverwrite; Components: tools

Source: Tools\versioncheck\*.py;   DestDir: "{app}\Tools\versioncheck"; CopyMode: alwaysoverwrite; Components: tools
Source: Tools\versioncheck\README; DestDir: "{app}\Tools\versioncheck"; DestName: README.txt;      CopyMode: alwaysoverwrite; Components: tools

Source: Tools\idle\*.py;      DestDir: "{app}\Tools\idle";       CopyMode: alwaysoverwrite; Components: tools
Source: Tools\idle\idle.pyw;  DestDir: "{app}\Tools\idle";       CopyMode: alwaysoverwrite; Components: tools
Source: Tools\idle\*.txt;     DestDir: "{app}\Tools\idle";       CopyMode: alwaysoverwrite; Components: tools
Source: Tools\idle\Icons\*.*; DestDir: "{app}\Tools\idle\Icons"; CopyMode: alwaysoverwrite; Components: tools

Source: Tools\compiler\*.py;           DestDir: "{app}\Tools\compiler";                       CopyMode: alwaysoverwrite; Components: tools
Source: Tools\compiler\README;         DestDir: "{app}\Tools\compiler"; DestName: README.txt; CopyMode: alwaysoverwrite; Components: tools
Source: Tools\compiler\compiler\*.py;  DestDir: "{app}\Tools\compiler\compiler";              CopyMode: alwaysoverwrite; Components: tools
Source: Tools\compiler\compiler\*.txt; DestDir: "{app}\Tools\compiler\compiler";              CopyMode: alwaysoverwrite; Components: tools

Source: Tools\pynche\*.py;       DestDir: "{app}\Tools\pynche";   CopyMode: alwaysoverwrite; Components: tools
Source: Tools\pynche\pynche.pyw; DestDir: "{app}\Tools\pynche";   CopyMode: alwaysoverwrite; Components: tools
Source: Tools\pynche\X\*.txt;    DestDir: "{app}\Tools\pynche\X"; CopyMode: alwaysoverwrite; Components: tools
Source: Tools\pynche\README;     DestDir: "{app}\Tools\pynche";   DestName: README.txt;      CopyMode: alwaysoverwrite; Components: tools
Source: Tools\pynche\pynche;     DestDir: "{app}\Tools\pynche";   DestName: pynche.py;       CopyMode: alwaysoverwrite; Components: tools

Source: html\*.*; DestDir: "{app}\Doc"; CopyMode: alwaysoverwrite; Flags: recursesubdirs; Components: docs

Source: Lib\test\*.py;         DestDir: "{app}\Lib\test";        CopyMode: alwaysoverwrite; Components: test
Source: Lib\test\*.uue;        DestDir: "{app}\Lib\test";        CopyMode: alwaysoverwrite; Components: test
Source: Lib\test\*.xml;        DestDir: "{app}\Lib\test";        CopyMode: alwaysoverwrite; Components: test
Source: Lib\test\*.out;        DestDir: "{app}\Lib\test";        CopyMode: alwaysoverwrite; Components: test
Source: Lib\test\audiotest.au; DestDir: "{app}\Lib\test";        CopyMode: alwaysoverwrite; Components: test
Source: Lib\test\output\*.*;   DestDir: "{app}\Lib\test\output"; CopyMode: alwaysoverwrite; Components: test

[Icons]
Tasks: startmenu; Name: "{group}\IDLE (Python GUI)"; Filename: "{app}\pythonw.exe"; WorkingDir: "{app}"; Parameters: """{app}\Tools\idle\idle.pyw"""; Components: tools
Tasks: startmenu; Name: "{group}\Module Docs"; Filename: "{app}\pythonw.exe"; WorkingDir: "{app}"; Parameters: """{app}\Tools\Scripts\pydoc.pyw"""; Components: tools
Tasks: startmenu; Name: "{group}\Python (command line)"; Filename: "{app}\python.exe"; WorkingDir: "{app}"; Components: main
Tasks: startmenu; Name: "{group}\Python Manuals"; Filename: "{app}\Doc\index.html"; WorkingDir: "{app}"; Components: docs

[Registry]
; Register .py
Tasks: extensions; Root: HKCR; Subkey: ".py"; ValueType: string; ValueName: ""; ValueData: "Python File"; Flags: uninsdeletevalue
Tasks: extensions; Root: HKCR; Subkey: ".py"; ValueType: string; ValueName: "Content Type"; ValueData: "text/plain"; Flags: uninsdeletevalue
Tasks: extensions; Root: HKCR; Subkey: "Python File"; ValueType: string; ValueName: ""; ValueData: "Python File"; Flags: uninsdeletekey
Tasks: extensions; Root: HKCR; Subkey: "Python File\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\Py.ico"
Tasks: extensions; Root: HKCR; Subkey: "Python File\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\python.exe"" ""%1"" %*"

; Register .pyc
Tasks: extensions; Root: HKCR; Subkey: ".pyc"; ValueType: string; ValueName: ""; ValueData: "Python CompiledFile"; Flags: uninsdeletevalue
Tasks: extensions; Root: HKCR; Subkey: "Python CompiledFile"; ValueType: string; ValueName: ""; ValueData: "Compiled Python File"; Flags: uninsdeletekey
Tasks: extensions; Root: HKCR; Subkey: "Python CompiledFile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\pyc.ico"
Tasks: extensions; Root: HKCR; Subkey: "Python CompiledFile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\python.exe"" ""%1"" %*"

; Register .pyo
Tasks: extensions; Root: HKCR; Subkey: ".pyo"; ValueType: string; ValueName: ""; ValueData: "Python CompiledFile"; Flags: uninsdeletevalue

; Register .pyw
Tasks: extensions; Root: HKCR; Subkey: ".pyw"; ValueType: string; ValueName: ""; ValueData: "Python NoConFile"; Flags: uninsdeletevalue
Tasks: extensions; Root: HKCR; Subkey: ".pyw"; ValueType: string; ValueName: "Content Type"; ValueData: "text/plain"; Flags: uninsdeletevalue
Tasks: extensions; Root: HKCR; Subkey: "Python NoConFile"; ValueType: string; ValueName: ""; ValueData: "Python File (no console)"; Flags: uninsdeletekey
Tasks: extensions; Root: HKCR; Subkey: "Python NoConFile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\Py.ico"
Tasks: extensions; Root: HKCR; Subkey: "Python NoConFile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\pythonw.exe"" ""%1"" %*"

