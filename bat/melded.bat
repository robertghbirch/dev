@echo off
IF "%2"=="" (
	echo Usage: n file
) ELSE (
	"c:\Program Files (x86)\Meld\Meld.exe" %1 %2
)
