b=2

while [ $b -lt 100 ]
do
   echo "FIFO"
   ./virtmem 100 $b fifo sort
   ./virtmem 100 $b fifo focus
   ./virtmem 100 $b fifo scan
   b=`expr $b + 1`
done
