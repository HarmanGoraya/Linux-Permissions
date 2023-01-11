/*
 * Access - returns permissions of a given user or group for a file or directory
 *
 * Usage: ./Access [ -g ] name file1 ...
 *
 * Inputs: User enters a UID, GID, user name, or group name followed by one or more files/directories
 * Exit Code: (0) if valid input was given
 *            (1) otherwise
 *
 * 
 */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <cstring>

#define GROUP_NAME_INDEX 2
#define USER_NAME_INDEX 1

enum{is_name, is_number};

/*
 * Checks to see if the "name" argument is a name or non-negative number(id)
 *
 *
 * parameters:  char* NumberCheck: value that was given by the user for the "name" argument
 *
 *
 * returns:     is_name: if the value is a name
 *              is_number: if the value is a non-negative number
 */
int isnumber(char* NumberCheck)
{
     // Function uses isdigit as if there is a "-" it will be classified as a name
     // and not an id
     
    for(int i = 0; i < strlen(NumberCheck); i++){
        if(!isdigit(NumberCheck[i])){
            return is_name;
        }
    }

    return is_number;

}

/*
 * Returns print statements for a user/group for a file based on read, write, exec permissions
 *
 *
 * parameters:  int read: (1) if user/group has read permission (0) otherwise
 *              int write: (1) if user/group has write permission (0) otherwise
 *              int exec: (1) if user/group has execute permission (0) otherwise
 *
 *
 * returns:     std::string statement: string that summarizes permissions for a user/group for a file
 */
std:: string print_statement_file(int read, int write, int exec)
{
    std:: string statement;

    if(read && !write && !exec){
        statement = " can read the file ";
    }
    else if(read && write && !exec){
        statement = " can read and write the file ";
    }
    else if(read && !write && exec){
        statement = " can read and execute the file ";
    }
    else if(read && write && exec){
        statement = " can read, write, and execute the file ";
    }
    else if(!read && write && !exec){
        statement = " can write the file ";
    }
    else if(!read && write && exec){
        statement = " can write and execute the file ";
    }
    else if(!read && !write && exec){
        statement = " can execute the file ";
    }
    else{
        statement = " can do nothing with the file ";
    }

    return statement;

}

/*
 * Returns print statements for a user/group for a directory based on read, write, exec permissions
 *
 *
 * parameters:  int read: (1) if user/group has read permission (0) otherwise
 *              int write: (1) if user/group has write permission (0) otherwise
 *              int exec: (1) if user/group has execute permission (0) otherwise
 *
 *
 * returns:     std::string statement: string that summarizes permissions for a user/group for a directory
 */
std:: string print_statement_dir(int read, int write, int exec)
{
    std:: string statement;

    if(read && !write && !exec){
        statement = " can list the contents of the directory ";
    }
    else if(read && write && !exec){
        statement = " can list the contents of and modify the directory ";
    }
    else if(read && !write && exec){
        statement = " can list the contents of and search the directory ";
    }
    else if(read && write && exec){
        statement = " can list the contents of, modify, and search the directory ";
    }
    else if(!read && write && !exec){
        statement = " can modify the directory ";
    }
    else if(!read && write && exec){
        statement = " can modify and search the directory ";
    }
    else if(!read && !write && exec){
        statement = " can search the directory ";
    }
    else{
        statement = " can do nothing with the directory ";
    }

    return statement;

}

/*
 * This function gets the owner permissions by extracting the user permission bits
 * with flags S_IRUSR(User Read), S_IWUSR(User Write), and S_IXUSR(User execute)
 * http://www.skrenta.com/rt/man/stat.2.html
 *
 *
 * paramaters:  structstat: stat structure that holds the protection mode for the file/directory (.st_mode)
 *              dir:        Used to keep track whether the current file is a directory or other file;
 *                          0 = Non-directory file, non-zero integer means it is a directory file
 *
 *
 * return:      std::string res: string that summarizes permissions for a user/group for a file/directory
 */
std::string get_owner_permissions(struct stat structstat, int dir = 0)
{

    int read = 0;
    int write = 0;
    int execute = 0;
    std::string res;

    if (structstat.st_mode & S_IRUSR){
        read = 1;
    }
    if (structstat.st_mode & S_IWUSR) {
        write = 1;
    }
    if (structstat.st_mode & S_IXUSR) {
        execute = 1;
    }

    if(dir){
        res = print_statement_dir(read, write, execute);
    }
    else{
        res = print_statement_file(read, write, execute);
    }

    return res;

}

/*
 * This function gets the group permissions by extracting the group permission bits
 * with flags S_IRGRP(Group Read), S_IWGRP(Group Write), and S_IXGRP(Group Execute)
 *  
 *
 * paramaters:  structstat: stat structure that holds the protection mode for the file/directory (.st_mode)
 *              dir:        Used to keep track whether the current file is a directory or other file;
 *                          0 = Non-directory file, non-zero integer means it is a directory file
 *
 *
 * return:      std::string res: string that summarizes permissions for a user/group for a file/directory
 */
std::string get_group_permissions(struct stat structstat, int dir = 0){

    int read = 0;
    int write = 0;
    int execute = 0;
    std::string res; 


    if (structstat.st_mode & S_IRGRP){
        read = 1;
    }
    if (structstat.st_mode & S_IWGRP){
        write = 1;

    }
    if (structstat.st_mode & S_IXGRP){
        execute = 1;

    }

    if(dir){
        res = print_statement_dir(read, write, execute);
    }
    else{
        res = print_statement_file(read, write, execute);
    }

    return res;

}

/*
 * This function gets the others permissions by extracting the others permission bits
 * with flags S_IROTH(Others Read), S_IWOTH(Others Write), and S_IXOTH(Others execute)
 *
 *
 * paramaters:  structstat: stat structure that holds the protection mode for the file/directory (.st_mode)
 *              dir:        Used to keep track whether the current file is a directory or other file;
 *                          0 = Non-directory file, non-zero integer means it is a directory file
 *
 * return:      std::string res: string that summarizes permissions for a user/group for a file/directory
 */
std::string get_other_permissoins(struct stat structstat, int dir = 0){

    int read = 0;
    int write = 0;
    int execute = 0;

    std::string res;

    if (structstat.st_mode & S_IROTH){
        read = 1;
    }
    if (structstat.st_mode & S_IWOTH){
        write = 1;

    }
    if (structstat.st_mode & S_IXOTH){
        execute = 1;
    }

    if(dir){
        res = print_statement_dir(read, write, execute);
    }
    else{
        res = print_statement_file(read, write, execute);
    }

    return res;

}

/*
 *
 * Function gets permissions for UID or GID that has no associated name
 *
 *
 * parameters:  char** args: Command line arguments
 *              int ind: holds the the index of the current file/directory in args
 *              int type:= 0 (User), type:= 1 (Group)
 *
 * return:      int : (0) Success
 *                    (-1) No such file or directory
 */
int no_name(char** args, int ind, int id, int type)
{

    struct stat buffer;
    std::string permissons;
    int status;
    int dir = 0;

    status = stat(args[ind], &buffer);

    if(status == -1){
        std::cerr<<args[ind]<<": No such file or directory\n";
        return -1;
    }

    dir = S_ISDIR(buffer.st_mode);

    // User ID
    if(type == 0){
        if(uid_t(id) == buffer.st_uid){
            permissons = get_owner_permissions(buffer,dir);
        }
        else{
            permissons = get_other_permissoins(buffer, dir);
        }
        std::cout<<"The user with UID "<<args[USER_NAME_INDEX]<<permissons<<args[ind]<<std::endl;
    }
    // Group ID
    else if(type == 1){
        if(gid_t(id) == buffer.st_gid){
            permissons = get_group_permissions(buffer, dir);
        }
        else{
            permissons = get_other_permissoins(buffer, dir);
        }
        std::cout<<"Members of the group with GID "<<args[GROUP_NAME_INDEX]<<permissons<<args[ind]<<std::endl;

    }

    return 0;

}

/*
 * Gets permissions for a group
 *
 * parameters:  char** Input: command line arguments
 *              int args: number of command line arguments
 *              int index: value maps to the current file/directory index in Input
 *
 * returns:     (-1): If there is an error such as: 
 *                                                      (1) file/directory not existing
 *                                                      (2) there being no file/directory
 *              (0): Success                                      
 */
int group(char** Input, int args, int index){

    struct group* Entry;
    int NumberCheck = isnumber(Input[GROUP_NAME_INDEX]); // check whether "name" variable is a Group Name or GID
    int dir = 0; // Keeps track if a input is a file(0) or a directory(1)

    if(NumberCheck == is_name){
            /* If name use the getgrnam function to get group structure that points to GID */
        if((Entry = getgrnam(Input[GROUP_NAME_INDEX])) ==  NULL){
            std::cerr<<Input[GROUP_NAME_INDEX]<<": no such group\n";
            exit(1);
        }

    }
    else if(NumberCheck == is_number){
        std::string Con(Input[GROUP_NAME_INDEX]); // Convert the Group ID into a C++ string
        int Convert;

        
        try { // Try to convert the string to an integer
            Convert = std::stoi(Con);
        }
        catch (std::out_of_range &e) {
            std::cerr<<Input[GROUP_NAME_INDEX]<<": number too big\n";
            exit(1);

        }
        if((Entry = getgrgid(Convert)) == NULL){
	   
	    if(args < 4){
                std::cerr<<Input[0]<<": need at least one file or directory!\n";
       		exit(1);
    	    }	
            no_name(Input, index, Convert, 1); // Could not find name to match GID
            return 0;
        }

    }


    if(args < 4){
        std::cerr<<Input[0]<<": need at least one file or directory!\n";
        exit(1);
    }

    struct stat buffer;
    int status;

    status = stat(Input[index], &buffer);

    if(status == -1){
        std::cerr<<Input[index]<<": No such file or directory\n";
        return -1;
    }

    std::string permissons;

    dir = S_ISDIR(buffer.st_mode);


    if(Entry->gr_gid == buffer.st_gid){
        permissons = get_group_permissions(buffer, dir);
    }

    else{
        permissons = get_other_permissoins(buffer, dir);
    }

    std::cout<<"Members of the group "<<Entry->gr_name<<" (GID "<<Entry->gr_gid<<")"<< permissons<< Input[index]<<std::endl;


    return 0;

}

/*
 * Gets permissions for a user
 *
 * parameters:  char** Input: command line arguments
 *              int args: number of command line arguments
 *              int index: value maps to the current file/directory index in Input
 *
 * returns:     (-1): If there is an error such as:
 *                                                      (1) file/directory not existing
 *                                                      (2) there being no file/directory
 *              (0): Success
 */
int user(char** Input, int args, int index){

    struct passwd* Entry;
    int NumberCheck = isnumber(Input[USER_NAME_INDEX]);
    int dir = 0;

    if(NumberCheck == is_name){
	// Use the function getpwnam is the "name" is a name
        if((Entry = getpwnam(Input[USER_NAME_INDEX])) ==  NULL){
            std::cerr<<Input[USER_NAME_INDEX]<<": no such user"<<std::endl;
            exit(1);

        }

    }

    else if(NumberCheck == is_number){
        std::string Con(Input[USER_NAME_INDEX]);
        int Convert;

        try {
            Convert = std::stoi(Con);
        }
        catch (std::out_of_range &e) {
            std::cerr<<Input[USER_NAME_INDEX]<<": number too big\n";
            exit(1);

        }
 	// Call getpwuid if "name" is an id
        if((Entry = getpwuid(Convert)) == NULL){
	    if(args < 3){
                std::cerr<<Input[0]<<": need at least one file or directory!\n";
                exit(1);
            }
            no_name(Input, index, Convert, 0); // If there is no name associated with the id call the no_name function
            return 0;
        }

    }

    if(args < 3){
        std::cerr<<Input[0]<<": need at least one file or directory!\n";
        exit(1);
    }

    struct stat buffer;
    int status;

    status = stat(Input[index], &buffer);

    if(status == -1){
        std::cerr<<Input[index]<<": No such file or directory\n";
        return -1;
    }

    std::string permissons;

    dir = S_ISDIR(buffer.st_mode);

    if(Entry->pw_uid == 0){ // Special privileges if user has UID of 0
        if(dir){
            std::cout<<"The user "<<Entry->pw_name<<" (UID "<<Entry->pw_uid<<
            ") can list the contents of, modify, and search the directory "<< Input[index]<<"\n";

        }
        else{
            std::cout<<"The user "<<Entry->pw_name<<" (UID "<<Entry->pw_uid<<
                     ") can read, write, and execute the file "<< Input[index]<<"\n";
        }

        return 0;
    }

    if(Entry->pw_uid == buffer.st_uid){
        permissons = get_owner_permissions(buffer, dir);
    }

    else if(Entry->pw_gid == buffer.st_gid){
        permissons = get_group_permissions(buffer, dir );
    }

    else{ // Check to see if the UID has a secondary GID that is the file/directory's GID
        int max = NGROUPS_MAX;
        gid_t group_names[max];
        int in_group = 0;

        getgrouplist(Entry->pw_name, Entry->pw_gid, group_names, &max);
        for(int i = 0; i < max;i++){
            // There is a match
            if(group_names[i] == buffer.st_gid){
                permissons = get_group_permissions(buffer, dir );
                in_group++;
                break;
            }

        }
        
        if(!in_group){ // If not get other permissions
            permissons = get_other_permissoins(buffer, dir);
        }
    }

    std::cout<<"The user "<<Entry->pw_name<<" (UID "<<Entry->pw_uid<<")"<< permissons<< Input[index]<<"\n";

    return 0;

}

int main(int argc, char **argv) {

    if(argv[0] == NULL){
        std::cerr<<"access (argv[0] NULL!)\n";
        exit(1);

    }

    if(argc < 2){

        std::cerr<<"usage: "<<argv[0]<<" [ -g ] name file1 ...\n";
        exit(1);
    }


    if(strcmp(argv[1], "-g") == 0){


        if(argc == 2){
            std::cerr<<"Usage: "<<argv[0]<<": -g requires group name\n";
            exit(1);
        }

	/* In this case the group function will return an error 
	 * For either the Name not being valid or there not being a file
	 * We call the function knowing there is no file/directory as the function
	 * checks if the name is valid which has precedence over no file/directory error
	 */
        if(argc == 3){
            group(argv, argc, argc - 1);
        }

	/* We want to loop through all of the files listed
	 * We know that after the error checks the file for groups will begin 
	 * on index 3 and we must not loop through the first three args
	 */
        else{
            for(int i = 0; i < argc - 3; i++){
                group(argv, argc, i + 3);
            }
        }
    }
    else{
	/* This call will also return an error either the name does not exist or
	 * there is not file/directory
	 */
        if(argc == 2){
	    user(argv, argc, argc - 1);
        }
	
	/* Similarly we know that the files will begin at index 2 for Users, so 
	 * we loop through those indicies
	 */
	else{
            for(int i = 0; i < argc - 2; i++){
                user(argv, argc, i + 2);
            }
        }
	
    }

    return 0;
}
