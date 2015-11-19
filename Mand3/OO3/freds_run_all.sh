b=2

while [ $b -lt 100 ]
do
   echo "NumberOfFrames: $b"
   echo "FIFO"
   ./virtmem 100 $b fifo sort
   ./virtmem 100 $b fifo focus
   ./virtmem 100 $b fifo scan
   echo "RAND" 
   ./virtmem 100 $b rand sort
   ./virtmem 100 $b rand focus
   ./virtmem 100 $b rand scan
   echo "CUSTOM" 
   ./virtmem 100 $b custom sort
   ./virtmem 100 $b custom focus
   ./virtmem 100 $b custom scan
   b=`expr $b + 1`
done