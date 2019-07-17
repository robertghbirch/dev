#SingleInstance force
#space::Run www.google.com
#n::
RunWait Notepad
MsgBox The user has finished (Notepad has been closed).
return
^!c::Run calc.exe
::imho::in my humble opinion