#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int file1, file2;
    char buf[BUFSIZ];

// check if the number of arguments is correct
    if (argc != 3) {
        printf("Insufficient parameters passed.\n");
        return -1;
    }
    
// output error message if the input file cannot be opened or read
    file1 = open(argv[1], O_RDONLY);
    if (file1 == -1) {
        // print error message
        printf("ERROR: Cannot open input file.");
        return -1;
    }

// output error message if the output file cannot be opened/created, written or truncated
    file2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file2 == -1) {
        printf("ERROR: Cannot create or write to destination file.");
        return -1;
    }

// read from the input file and write to the output file
    int n = read(file1, buf, sizeof(buf));
    int write_tofile = write(file2, buf, n);
    
    if (write_tofile != n) {
        printf("");
        return -1;
        }
    
    else {
        printf("The contents of file %s have been successfully copied into the %s file\n", argv[1], argv[2]);
    }   
    
    if (n == -1) {
        printf("Write error.");
        return -1;
    }

// close the input and output files
    close(file1);
    close(file2);


    return 0;
}

