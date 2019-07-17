@echo off
IF "%1"=="" (
	echo Usage: n file
) ELSE (
	notepad++ %1
)