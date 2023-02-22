#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 11
#define ROWS 9
#define COLS 9
#define SUBGRID_SIZE 3

typedef struct {
    int row;
    int col;
} parameters;

int sudoku[ROWS][COLS];
int result[NUM_THREADS];

void *check_column(void *param) {
    parameters *p = (parameters *)param;
    int col = p->col;
    int valid[ROWS+1] = {0};
    for (int i = 0; i < ROWS; i++) {
        int num = sudoku[i][col];
        if (valid[num]) {
            result[0] = 0;
            pthread_exit(NULL);
        }
        valid[num] = 1;
    }
    result[0] = 1;
    pthread_exit(NULL);
}

void *check_row(void *param) {
    parameters *p = (parameters *)param;
    int row = p->row;
    int valid[COLS+1] = {0};
    for (int j = 0; j < COLS; j++) {
        int num = sudoku[row][j];
        if (valid[num]) {
            result[1] = 0;
            pthread_exit(NULL);
        }
        valid[num] = 1;
    }
    result[1] = 1;
    pthread_exit(NULL);
}

void *check_subgrid(void *param) {
    parameters *p = (parameters *)param;
    int row = p->row;
    int col = p->col;
    int valid[ROWS+1] = {0};
    for (int i = row; i < row+SUBGRID_SIZE; i++) {
        for (int j = col; j < col+SUBGRID_SIZE; j++) {
            int num = sudoku[i][j];
            if (valid[num]) {
                result[row/3*3+col/3+2] = 0;
                pthread_exit(NULL);
            }
            valid[num] = 1;
        }
    }
    result[row/3*3+col/3+2] = 1;
    pthread_exit(NULL);
}

int main() {
    FILE *fp;
    fp = fopen("sample_in_sudoku.txt", "r");
    if (fp == NULL) {
        printf("Error: cannot open file.\n");
        exit(1);
    }

    // Read the Sudoku puzzle from file into the sudoku array
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fscanf(fp, "%d", &sudoku[i][j]);
        }
    }

    // Initialize thread array and parameter array
    pthread_t threads[NUM_THREADS];
    parameters params[NUM_THREADS];

    // Create threads for checking each column
    for (int i = 0; i < COLS; i++) {
        params[i].row = 0;
        params[i].col = i;
        pthread_create(&threads[i], NULL, check_column, (void *)&params[i]);
    }

    // Create threads for checking each row
    for (int i = 0; i < ROWS; i++) {
        params[COLS + i].row = i;
        params[COLS + i].col = 0;
        pthread_create(&threads[COLS + i], NULL, check_row, (void *)&params[COLS + i]);
    }

    // Create threads for checking each subgrid
    for (int i = 0; i < SUBGRID_SIZE; i++) {
        for (int j = 0; j < SUBGRID_SIZE; j++) {
            int idx = 2 + i*SUBGRID_SIZE + j;
            params[COLS + ROWS + idx].row = i*SUBGRID_SIZE;
            params[COLS + ROWS + idx].col = j*SUBGRID_SIZE;
            pthread_create(&threads[COLS + ROWS + idx], NULL, check_subgrid, (void *)&params[COLS + ROWS + idx]);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // print the sudoku
    printf("Sudoku:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }

    // Check the result array to see if the Sudoku puzzle is valid
    int valid = 1;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (result[i] == 0) {
            valid = 0;
            break;
        }
    }

    if (valid) {
        printf("The Sudoku puzzle is valid!\n");
    } else {
        printf("The Sudoku puzzle is not valid.\n");
    }

    return 0;
}
   
