gcc ex1.c -o ex1
chmod 777 ex1
./ex1 &

sleep 1 # without that, the program can't keep up with writing a new password before it's taken

# get the process ID
pid=$(cat /home/erzhe/week08/ex1.pid)

# get the memory address of the heap
addr=$(cat /proc/$pid/maps | grep heap | awk '{print $1}')

start=$(echo $addr | cut -d'-' -f1)
end=$(echo $addr | cut -d'-' -f2)

start="0x$start"
end="0x$end"

echo "Address: $addr"

# extract the password from the process's memory
pass=$(sudo xxd -s $start -l $(($end-$start)) /proc/$pid/mem | grep "pass:")

if [ -z "$pass" ]; then
    echo "Failed to extract the password."
    sudo kill -9 $pid
    rm ex1
    exit 1
fi

# cut the password from the entire data string
pass=$(echo $pass | cut -d':' -f3 | cut -c1-8)
echo "Password: $pass"

sudo kill -9 $pid
rm ex1
