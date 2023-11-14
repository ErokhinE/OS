# "Inode of ex1:"
ls -i ex1
# "Number of blocks and block size of ex1:"
stat --printf="Blocks: %b\nBlock size: %B\nTotal size: %s bytes\nPermissions: %A\n" ex1
cp ex1 ex2
#"Number of links for ex2:"
stat -c "%h" ex2
#"Inode of ex1:"
ls -i ex1
#"Inode of ex2:"
ls -i ex2
#"Files with 3 links in /etc:"
stat -c "%h - %n" /etc/* | grep "^3"
# Explanation
#"The link count of a file represents the total number of existing links that point to the file. It signifies the number of directory entries that reference the underlying inode of the file. When a file is initially created, it has a link count of one. Each additional hard link created for the file increments this count by one."
