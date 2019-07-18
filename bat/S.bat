@echo off

IF "%1"=="" (    
  echo Usage: n file
) ELSE (
  
  set "var=%cd%"
  SET c=c:\SAFE\dev\bat\%1% 
  REM echo %c%

	IF exist %c% (
	  echo Error: %c% exists.
	) ELSE (
      echo cd %var% > %c%
	)
  
)