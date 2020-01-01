#! /usr/bin/env python
import os,sys
import win32clipboard as w
import win32con
#CurrentDir = os.getcwd()
#CurrentDir = sys.path[0]
#CurrentDir = os.path.abspath()
CurrentDir = os.path.abspath("")
fp = open(CurrentDir+"\\apollo.map",'r')
def settext(aString):
	w.OpenClipboard()
	w.EmptyClipboard()
	w.SetClipboardData(win32con.CF_UNICODETEXT, aString)
	w.CloseClipboard()
	
for line in fp:
	if "_SEGGER_RTT" in line:
		split = line.split()[1]
		settext(split)
fp.close()





