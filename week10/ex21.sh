file /dev/*
file /etc/*
dir_count=$(ls -l /etc | grep ^d | wc -l)
echo "num of directories in /etc: $dir_count"
echo -e '#include <stdio.h>\nint main() {\n  printf("Привет, мир!");\n  return 0;\n}' > ex1.c
#"Type of ex1.c before:"
file ex1.c
gcc -o ex1 ex1.c
#"Type of ex1 after:"
file ex1

