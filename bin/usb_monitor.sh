#!/bin/bash

KRED=`tput setaf 1`
KGREEN=`tput setaf 2`
KYELLOW=`tput setaf 3`
KWHITE=`tput setaf 7`
KCYAN=`tput setaf 6`
RESET=`tput sgr0`

lsusb > /tmp/c1
while [ true ]
do lsusb > /tmp/c2 
 echo -n $RESET
 diff /tmp/c1 /tmp/c2
 echo -n $KYELLOW
 ls /dev/video* 2>/dev/null 
 echo -n $KWHITE
 date
 echo -n $KCYAN
 echo -n CAMS:
 ls /dev/video* 2>/dev/null | wc -w
 #sleep 1
done
