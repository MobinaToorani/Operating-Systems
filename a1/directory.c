#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include <dirent.h>
#include <unistd.h>

#define LENGTH 50
void print_options();

//funtion to store the options
void print_options(){
    printf("Select the option(s) appropriately by entering the number:\n\
            \tEnter 1 for creating a directory\n\
            \tEnter 2 for removing a directory\n\
            \tEnter 3 for printing working directory\n\
            \tEnter 4 for changing directory one level\n\
            \tEnter 5 for reading the contents of directory\n\
            \tEnter 6 for closing the current directory\n\
            \tEnter q to exit the program\n");

    return;
}

int main() {
    char input = ' ';

    // if the user enters q, the program exits
    while(input != 'q') {
        // flush the buffer
        fflush(stdout);
        
        print_options();
        scanf(" %c", &input);

        if(input == '1') {
            printf("Enter the Directory name you want to create:\n");
            char dir_name[LENGTH];
            scanf("%s", dir_name);

            int dir = mkdir(dir_name, 0777);

            if(dir == 0) {
                printf("Directory is Created Successfully.\n");
            } else {
                printf("Error\n");
            }
        }

        else if(input == '2') {
            printf("Enter the Directory name you want to remove:\n");
            char dir_name[LENGTH];
            scanf("%s", dir_name);

            int dir = rmdir(dir_name);

            if(dir == 0) {
                printf("Directory is removed Successfully.\n");
            } else {
                printf("Error\n");
            }  
        }

        else if(input == '3') {
            char cwd[200];
            getcwd(cwd, sizeof(cwd));
            printf("Current Working Directory is : %s :\n", cwd);
            }

        else if(input == '4') {
            char dir_name[LENGTH];
            char cwd[200];
            getcwd(cwd, sizeof(cwd));
            printf("Working Directory Before Operation: %s :\n", cwd);
            
            // change the directory to the parent directory 
            int dir = chdir("..");

            if (dir == 0){
                printf("Directory Changed Successfully\n");
            }
            else {
                printf("Error");
            }
            // print the current working directory after the operation
            getcwd(dir_name, sizeof(dir_name));
            printf("Working Directory After Operation: %s\n", dir_name);
        }
        else if(input == '5') {
            char dir_name[LENGTH];
            getcwd(dir_name, sizeof(dir_name));

            DIR *dir;
            struct dirent *ent;
            
            // open the directory
            if ((dir = opendir(dir_name)) != NULL) {
                // print all the files and directories within directory 
                while ((ent = readdir (dir)) != NULL) {
                    printf("%s\n", ent->d_name);
                } 
            }
            else {
                printf("Error\n");
                return -1;
            }


        }
        else if(input == '6') {
            char dir_name[LENGTH];
            getcwd(dir_name, sizeof(dir_name));
            // open the directory to close it
            DIR *odir = opendir(dir_name);
            closedir(odir);

            printf("Directory is closed Successfully.\n");
        }
    }

    return 0;
}