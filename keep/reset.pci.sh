for i in /sys/bus/pci/drivers/xhci_hcd/*:*
do 
  echo "${i%/*}"/${i##*/} 

  echo find
  cd $i

  MOUSE_KEYBOARD=0

  # for j in `find . -name manufacturer`; do cat $j  | grep e-con ; done
  #echo -n "PWD : " 
  # pwd
  # find . -name | grep Logitech >/dev/null
  for j in `find . -name manufacturer`
  do 
      cat $j | grep Logitech >/dev/null 1>&2
      if [ $? -eq 0 ] 
      then
  	MOUSE_KEYBOARD=1
      fi
  done

  if [ $MOUSE_KEYBOARD -eq 0 ] 
  then
	echo "NON-MOUSE"
  	echo ${i##*/}  "${i%/*}"/unbind
  	#echo ${i##*/} > "${i%/*}"/unbind
	#sleep 1
  	#echo ${i##*/} >  "${i%/*}"/bind
  fi

done

exit


cd /sys/bus/pci/drivers/pcieport/

for i in  ????:??:??.?
do 
  echo "unbinding" $i
  echo -n "$i" > unbind
  sleep 1
  echo "binding" $i
  echo -n "$i" > bind
done
0000:00:14.0
/sys/bus/pci/drivers/xhci_hcd
0000:03:00.0
/sys/bus/pci/drivers/xhci_hcd
0000:04:00.0
/sys/bus/pci/drivers/xhci_hcd
0000:05:00.0
/sys/bus/pci/drivers/xhci_hcd
0000:06:00.0
/sys/bus/pci/drivers/xhci_hcd
0000:08:00.0
/sys/bus/pci/drivers/xhci_hcd
0000:0b:00.0
/sys/bus/pci/drivers/xhci_hcd
0000:0c:00.0
/sys/bus/pci/drivers/xhci_hcd
