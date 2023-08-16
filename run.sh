echo 3 > /proc/sys/vm/drop_caches
./multi_test
./traverse
dmesg
