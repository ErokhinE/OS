gcc ex3.c -o ex3
./ex3 3 &
pstree
sleep 1
pstree
./ex3 5 &
pstree
sleep 2
pstree
sleep 2
pstree
