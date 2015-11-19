echo "Custom sort"
./stat.sh custom sort > stat/custom_sort.txt
echo "Custom focus"
./stat.sh custom focus > stat/custom_focus.txt
echo "Custom scan"
./stat.sh custom scan > stat/custom_scan.txt
echo "FIFO sort"
./stat.sh fifo sort > stat/FIFO_sort.txt
echo "FIFO focus"
./stat.sh fifo focus > stat/FIFO_focus.txt
echo "FIFO scan"
./stat.sh fifo scan > stat/FIFO_scan.txt
echo "rand sort"
./stat.sh rand sort > stat/rand_sort.txt
echo "rand focus"
./stat.sh rand focus > stat/rand_focus.txt
echo "rand scan"
./stat.sh rand scan > stat/rand_scan.txt
echo "randopt sort"
./stat.sh randopt sort > stat/randopt_sort.txt
echo "randopt focus"
./stat.sh randopt focus > stat/randopt_focus.txt
echo "randopt scan"
./stat.sh randopt scan > stat/randopt_scan.txt