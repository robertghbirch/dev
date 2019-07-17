$port= new-Object System.IO.Ports.SerialPort COM9,115200,None,8,one
$port.open()

while (1) 
{
	sleep(1)
	write("Check Input")
	$X=$port.ReadLine() 

	write("Input=[")
	write($X)
	write("]")

	if ($X -eq "B1")
	{
		write("W1W mDP")
		#Start-Process 'C:\windows\system32\notepad.exe'
		c:\"Program Files (x86)\Dell\Dell Display Manager"\ddm.exe /2:SetActiveInput DP /exit
	} Elseif($X -eq "B2"){
		write("W1L DP")
		#Start-Process 'C:\windows\system32\notepad.exe'
		c:\"Program Files (x86)\Dell\Dell Display Manager"\ddm.exe /2:SetActiveInput mDP /exit
	} Elseif($X -eq "B6"){
		write("W2W DP")
		#Start-Process 'C:\windows\system32\notepad.exe'
		c:\"Program Files (x86)\Dell\Dell Display Manager"\ddm.exe /1:SetActiveInput DP /exit
	} Elseif($X -eq "B7"){
		write("W2W DP")
		#Start-Process 'C:\windows\system32\notepad.exe'
		c:\"Program Files (x86)\Dell\Dell Display Manager"\ddm.exe /1:SetActiveInput mDP /exit
	} Else {
		write("NOT USED")
	}
	$X=""

}