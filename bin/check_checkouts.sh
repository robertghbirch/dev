for i in `echo CV1Firmware CV1FWToolchain MCBootloader MCRuntime`
do 
 echo -n $i "	"
 cd $i
 git rev-parse HEAD
 cd ..
done
