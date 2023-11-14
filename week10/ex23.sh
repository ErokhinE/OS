echo '$ for (( i=0; i<$1; i++ )); do echo $RANDOM >> $2; done' > gen.sh
bash gen.sh 10 ex1.txt
ln ex1.txt ex11.txt
ln ex1.txt ex12.txt
echo "Comparison of file contents:"
diff ex1.txt ex11.txt
diff ex1.txt ex12.txt
diff ex11.txt ex12.txt
# "The content of ex1.txt, ex11.txt, and ex12.txt is identical because they are hard links that point to the same inode. As a result, they all reference the same underlying data. Therefore, any modifications made to one of these files will be reflected in the others as well."
#"Inode numbers of files:"
ls -i ex1.txt ex11.txt ex12.txt > output.txt
#"The inode numbers for ex1.txt, ex11.txt, and ex12.txt are the same since they are hard links to the same file."
#"Disk usage of ex1.txt:"
du -h ex1.txt
ln ex1.txt ex13.txt
mv ex13.txt /tmp/
#"Links to ex1.txt in current path:"
find ./ -inum $(ls -i ex1.txt | awk '{print $1}')
#"Links to ex1.txt in root path (/):"
find / -inum $(ls -i ex1.txt | awk '{print $1}')
#"When searching in the current path, the search is restricted to the current directory and its subdirectories. This means that only the directories and files within the current directory and its subdirectories will be included in the search.
#On the other hand, when searching in the root path (/), the search encompasses the entire filesystem. It starts from the root directory (/) and includes all directories and files in the system. This means that the search will traverse through all directories and subdirectories, covering the entire filesystem."
#"Number of hard links for ex1.txt:"
ls -l ex1.txt | awk '{print $2}'
find ./ -inum $(ls -i ex1.txt | awk '{print $1}') -exec rm {} \;
