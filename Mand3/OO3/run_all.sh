echo "Fifo sort"
./stat.sh fifo sort > stat/fifo_sort.txt
echo "Fifo scan"
./stat.sh fifo scan > stat/fifo_scan.txt
echo "Fifo focus"
./stat.sh fifo focus > stat/fifo_focus.txt
echo "Rand sort"
./stat.sh rand sort > stat/rand_sort.txt
echo "Rand scan"
./stat.sh rand scan > stat/rand_scan.txt
echo "Rand focus"
./stat.sh rand focus > stat/rand_focus.txt
echo "Custom sort"
./stat.sh custom sort > stat/custom_sort.txt
echo "Custom scan"
./stat.sh custom scan > stat/custom_scan.txt
echo "Custom focus"
./stat.sh custom focus > stat/custom_focus.txt