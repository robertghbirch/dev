@echo off

IF "%1"=="" (    
  echo Usage: n file
) ELSE (
  
  set "var=%cd%"
  SET c=c:\SAFE\dev\bat\%1% 
  echo %c%

  IF exist %c% (
    echo Error: %c% already exists
  ) ELSE (
    SET f=c:\SAFE\dev\bat\%1
	echo %f%
    echo cd %var% > %f%
  )
)