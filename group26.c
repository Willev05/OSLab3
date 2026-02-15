/*
 * Lab 3 - Sudoku Validator
 * This is the code skeleton for Lab 3. If you do not need it, you may also build your code from scratch.
 *
 * Input : 9x9 integers from stdin (1..9, well-formed)
 * Output: print 1 if valid, else print 0
 * Do not print extra text (debug messages). Print only 0\n or 1\n.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int sudoku[9][9];

typedef struct {
    int row;
    int column;
    int index;
    int *results;
} parameters;

/* thread entry functions */
void *check_row(void *arg) {
    parameters *p = (parameters *)arg;
    //This thread will cover all rows, therefore, it will assume to be starting at [0][0]. Only results will be used on the struct.
    int numbers[9] = {0};
    for (int row = 0; row < 9; row++){
        for (int column = 0; column < 9; column++){
            int num = sudoku[row][column];
            if (num < 1 || num > 9) {
                return NULL;
            }
            if (numbers[num - 1]){
                return NULL;
            } 
            numbers[num - 1] = 1;
        }
        memset(numbers, 0, sizeof(numbers));
    }
    *(p->results) = 1;
    return NULL;
}

void *check_column(void *arg) {
    parameters *p = (parameters *)arg;
    //This thread will cover all columns, therefore, it will assume to be starting at [0][0]. Only results will be used on the struct.
    int nums[9] = {0};
    for (int column = 0; column < 9; column++){
        for (int row = 0; row < 9; row++) {
            int num = sudoku[row][column];
            if (num < 1 || num > 9) {
                return NULL;
            }
            if (nums[num - 1]){
                return NULL;
            } 
            nums[num - 1] = 1;
        }
        memset(nums, 0, sizeof(nums));
    }
    *(p->results) = 1;
    return NULL;
}

void *check_grid(void *arg) {
    parameters *p = (parameters *)arg;

    int nums[9] = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int value = sudoku[p->row + i][p->column + j];

            if (value < 1 || value > 9) {
                *(p->results) = 0;
                return NULL;
            }

            nums[value - 1]++;
        }
    }

    for (int i = 0; i < 9; i++) {
        if (nums[i] != 1) {
            *(p->results) = 0;
            return NULL;
        }
    }

    *(p->results) = 1;
    return NULL;
}

int main(void) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            scanf("%d", &sudoku[row][col]);
        }
    }

    int results[11] = {0};
    pthread_t threads[11];
    parameters params[11];

    //Create thread for the rows
    params[9].results = results + 9;
    pthread_create(&threads[9], NULL, check_row, params + 9);

    //Create thread for the columns
    params[10].results = results + 10;
    pthread_create(&threads[10], NULL, check_column, params + 10);

    //Create threads for the subgrids, need  0, 3, 6 for columns and rows.
    for (int i = 0; i < 9; i++){
        params[i].results = results + i;
        params[i].row = (i / 3) * 3;
        params[i].column = (i % 3) * 3;
        pthread_create(&threads[i], NULL, check_grid, params + i);
    }

    //Wait for all threads to complete and check their return value
    int validity = 1;
    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
        validity = (!validity || !results[i]) ? 0 : 1;
    }

    printf("%d\n", validity);

    return 0;
}