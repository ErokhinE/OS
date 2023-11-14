touch ex5.txt
chmod a-w ex5.txt
chmod ugo=rwx ex5.txt
chmod g=u ex5.txt
#"Permissions for ex5.txt (Numeric representation):"
stat -c "%a" ex5.txt
#"660 for ex5.txt means: read and write permissions for the owner (6), read and write permissions for the group (6), and no permissions for others (0)."
#"775 for ex5.txt means: read, write, and execute permissions for the owner (7), read, write, and execute permissions for the group (7), and read, write, and execute permissions for others (5)."
#"777 for ex5.txt means: read, write, and execute permissions for the owner (7), read, write, and execute permissions for the group (7), and read, write, and execute permissions for others (7)."

