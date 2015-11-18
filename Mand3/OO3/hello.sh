a=2
b=2

while [ $a -lt 100 ]
do
	while [ $b -lt 100 ]
	do
	   echo "FIFO"
	   ./virtmem 100 $b fifo sort
	   ./virtmem 100 $b fifo focus
	   ./virtmem 100 $b fifo scan
	   echo "CUSTOM" 
	   ./virtmem 100 $b custom sort
	   ./virtmem 100 $b custom focus
	   ./virtmem 100 $b custom scan
	   b=`expr $b + 1`
	done
	a=`expr $a + 1`
done