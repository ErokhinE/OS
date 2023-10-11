gcc -pthread ex3.c -o ex3

rm ex3_res.txt

{ time ./ex3 10000000 1; } 2>> ex3_res.txt
{ time ./ex3 10000000 2; } 2>> ex3_res.txt
{ time ./ex3 10000000 4; } 2>> ex3_res.txt
{ time ./ex3 10000000 10; } 2>> ex3_res.txt
{ time ./ex3 10000000 100; } 2>> ex3_res.txt
rm ex3
