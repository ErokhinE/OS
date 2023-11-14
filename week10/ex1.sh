gcc -o monitor monitor.c
gcc -o ex1 ex1.c
mkdir folder1
chmod 777 folder1
./monitor folder1 &
./ex1 folder1
#./ex1_test.sh

