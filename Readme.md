
ACCESS

Access is a program that finds the permissions for a user or group for files and directories.

USAGE:
	The command line input for the program is: access[ -g ] name  file1 …
	
	Where the -g flag denotes that the name is for a group
	The name argument can be given an user name, user id, group name, or group id.
	file1 can be a any type of file or directory


EXAMPLES:
	
	User_Examples:
		access user access.cpp
		access 0 access.o
		

	Group_Examples:
		access -g staff access.cpp access.o
		access -g 21 Makefile access.o access.cpp


SOURCES:
	
	http://www.skrenta.com/rt/man/stat.2.html - Information about flags to get permissions from st_mode
	https://linux.die.net/man/2/stat - general information about stat and Posix Macro S_ISDIR
	https://man7.org/linux/man-pages/man3/getgrnam.3.html - getgrnam function
	https://pubs.opengroup.org/onlinepubs/009695399/functions/getgrgid.html - getgrgid function
	https://linux.die.net/man/3/getpwuid - getpwuid function
	https://man7.org/linux/man-pages/man3/getpwnam.3.html - getpwnam function
	https://man7.org/linux/man-pages/man3/getgrouplist.3.html - getgrouplist function



	
	
	


