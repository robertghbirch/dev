#Persistent
SetTimer, PressTheKey, 30000
Return

PressTheKey:
DllCall("LockWorkStation")
Return