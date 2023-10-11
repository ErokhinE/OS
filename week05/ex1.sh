if [ $# -ne 1 ]; then
    echo "Usage: $0 <number of subscribers>"
    exit 1
fi

num_subscribers=$1

gcc -o publisher publisher.c
gcc -o subscriber subscriber.c


for ((i=1; i<=$num_subscribers; i++)); do
    gnome-terminal -- ./subscriber $i
done
sleep 1
gnome-terminal -- ./publisher $num_subscribers 

rm -r publisher
rm -r subscriber



