#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include <dirent.h>
#include <unistd.h>

#define LENGTH 50

int main() {
    char input;

    printf("Select the option(s) appropriately by entering the number:\n\
            \tEnter 1 for creating a directory\n\
            \tEnter 2 for removing a directory\n\
            \tEnter 3 for printing working directory\n\
            \tEnter 4 for changing directory one level\n\
            \tEnter 5 for grading the contents of directory\n\
            \tEnter 6 for closing the current directory\n\
            \tEnter q to exit the program\n");

    scanf("%c", &input);

    while(input != 'q') {
        if(input == '1') {
            printf("Enter the Directory name you want to create:\n");
            char dir_name[LENGTH];
            scanf("%s", dir_name);

            int dir = mkdir(dir_name, O_RDWR);

            if(dir) {
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

            if(dir) {
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
            int dir = chdir(dir_name);

            if (dir){
                printf("Directory Changed Successfully");
            }
            else {
                printf("Error");
            }
            getcwd(cwd, sizeof(cwd));
            printf("Working Directory After Operation: %s", dir_name);
        }
        else if(input == '5') {
            

        }
        else if(input == '6') {
            

        }
        else if(input == 'q') {
            
        }
        printf("Select the option(s) appropriately by entering the number:\n\
        \tEnter 1 for creating a directory\n\
        \tEnter 2 for removing a directory\n\
        \tEnter 3 for printing working directory\n\
        \tEnter 4 for changing directory one level\n\
        \tEnter 5 for grading the contents of directory\n\
        \tEnter 6 for closing the current directory\n\
        \tEnter q to exit the program\n");
    }
    return 0;
}