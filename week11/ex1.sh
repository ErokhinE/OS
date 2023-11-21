# Create a file
sudo dd if=/dev/zero of=lofs.img bs=1M count=50 
 
# Find a loop device 
loop_device=$(sudo losetup -f) 
 
# Setup the loop device
sudo losetup $loop_device lofs.img 
 
# Create a LOFS ext4 on the loop device 
sudo mkfs.ext4 $loop_device 
  
# Create a directory ./lofsdisk 
mkdir ./lofsdisk 
 
# Mount on the ./lofsdisk 
sudo mount $loop_device ./lofsdisk 
 
# Add two files file1, file2 to the LOFS 
echo "Evgenii" | sudo tee ./lofsdisk/file1 
echo "Erokhin" | sudo tee ./lofsdisk/file2 
 
# Parser function to return paths of shared libraries of a binary 
get_libs() { 
    ldd $1 | awk 'NF==4 && $3 ~ /^\// {print $3}; NF==2 && $1 ~ /^\// {print $1} '  
} 
 
# Use the function to get shared libraries of commands bash, cat, echo, ls 
libs_bash=$(get_libs $(which bash)) 
libs_cat=$(get_libs $(which cat)) 
libs_echo=$(get_libs $(which echo)) 
libs_ls=$(get_libs $(which ls)) 

# Compile ex1.c 
gcc ex1.c -o ex1 
 
# Add the commands to the LOFS 
sudo cp --parent  /bin/bash /bin/cat /bin/echo /bin/ls ./lofsdisk 
 
# Create the lib directory inside lofsdisk and copy the libraries 
sudo mkdir ./lofsdisk/lib 
sudo cp --parent $libs_bash $libs_cat $libs_echo $libs_ls ./lofsdisk

# Give roots to write read execute and copy executable file tp LOFS
chmod 777 ./lofsdisk/lib
chmod 777 ./lofsdisk/bin
chmod 777 ./lofsdisk/lib64
sudo cp ./ex1 ./lofsdisk


# Change the root directory and execute ex1 according to new root directory save in ex1.txt
sudo chroot ./lofsdisk /bin/bash -c "./ex1>/ex1.txt" 

# Make a symbolic link to the ex1.txt file
sudo ln -s ./lofsdisk/ex1.txt ./ex1.txt

# Set permissions
chmod 777 ./lofsdisk/ex1.txt
 
 
# Run the program ex1 and append the output to the ex1.txt file
./ex1 >> ex1.txt 
 
 
# Step 16: Display the difference between the outputs in both cases 
diff ex1.txt ./lofsdisk/ex1.txt > output_diff.txt

