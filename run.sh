LANG=C
export LANG=C
make
cd test0/
make clean
cp ../dmake .
make -pq | ./dmake

