rm -rf ./tmp
rm -rf tmp1
ln -s ./tmp tmp1
#"Listing before creating ./tmp:"
ls -li
mkdir ./tmp
#"Listing after creating ./tmp:" 
ls -li
bash gen.sh 5 ./tmp/ex1.txt
#"Contents of tmp1 (symbolic link):"
ls -l ./tmp1
mkdir -p ./tmp1/tmp2
ln -s $(realpath ./tmp) ./tmp1/tmp2
bash gen.sh 5 ./tmp1/tmp2/ex1.txt
#"Contents of ./tmp:"
ls -l ./tmp
#"Contents of ./tmp1/tmp2:"
ls -l ./tmp1/tmp2
#"Trying to access sub-folders:" # No such file or directory
ls -l ./tmp1/tmp2/tmp2/tmp2/tmp2
rm -rf ./tmp
#"Listing after deleting ./tmp:"
ls -li
rm -rf tmp1

