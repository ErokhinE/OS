gcc pager.c -o pager
gcc mmu.c -o mmu
gnome-terminal -- ./pager 4 2
pager_pid=$(pgrep pager)
gnome-terminal -- ./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pager_pid

