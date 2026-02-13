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
            if (numbers[num - 1]){
                *(p->results) = 0;
                return NULL;
            } 
            numbers[num - 1] = 1;
        }
        memset(numbers, 0, sizeof(numbers));
    }
    return NULL;
}

void *check_column(void *arg) {
    parameters *p = (parameters *)arg;

    // TODO

    return NULL;
}

void *check_grid(void *arg) {
    parameters *p = (parameters *)arg;

    // TODO

    return NULL;
}

int main(void) {
    /* Standard legal Sudoku, 
     * you can use it to test your program

    int test_sudoku[9][9] = {
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };*/

    // TODO: read sudoku[9][9] from stdin

    int *results;
    pthread_t threads[11];

    // TODO: create threads, cleanup resource, print results

    //Malloc all of our results
    results = calloc(11, sizeof(int));

    //Create thread for the rows
    parameters *param = malloc(sizeof(parameters));
    param->results = results + 10;
    pthread_create(&threads[10], NULL, check_row, param);

    //Create thread for the columns
    parameters *param = malloc(sizeof(parameters));
    param->results = results + 11;
    pthread_create(&threads[11], NULL, check_column, param);

    //Create threads for the subgrids
    for (int i = 0; i < 9; i++){
        parameters *param = malloc(sizeof(parameters));
        param->results = results + i;
        param->row = i / 3;
        param->column = i % 3;
        pthread_create(&threads[i], NULL, check_grid, param);
    }

    //Wait for all threads to complete and check their return value
    int validity = 1;
    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
        validity = (!validity || !results[i]) ? 0 : 1;
    }

    printf("%d\n");

    return 0;
}

/*
========================
Guidance / Hints
Content: Chenyu NING
Email: chenyu.ning@ontariotechu.ca
Format: ChatGPT

*This section contains background information intended to guide you when implementing your program. 
*If you do not need this guidance, you may delete it directly. 
*Based on these descriptions, you should be able to complete this assignment independently.
========================

--------------------------------------------------
1. Function Stack and Heap
--------------------------------------------------
Function stack:
- A LIFO (Last-In, First-Out) memory region automatically managed by the system
- When a function returns, the stack space associated with that function becomes invalid
- For example, local variables of a function are stored here

Heap:
- A memory region available during program execution
- Used to store data explicitly allocated by the programmer via malloc / calloc
- Data on the heap can be shared across functions or threads
- Must be manually released by the programmer using free

--------------------------------------------------
2. Parameter Passing to Threads
--------------------------------------------------
Threads execute concurrently, which means:
- The execution order of the main thread and child threads is unpredictable
- If multiple threads access the same “modifiable” variable p, you cannot guarantee what value p will have when a thread uses it

Examples:
- Incorrect approach: declaring a single `parameters p` and repeatedly modifying `p->row` to pass arguments to different threads; 
    threads may receive random values of `p->row` !
- Correct approach: ensure that each thread receives a parameter copy that will no longer be modified, 
    for example by creating a new `parameters p` in each loop iteration for exclusive use by one thread

--------------------------------------------------
3. Thread Functions
--------------------------------------------------
Thread function: a function that the operating system treats as the “entry point” of a new thread
- A thread function may call normal functions, but normal functions should not call thread functions
- Fixed declaration form of a thread function:
    void *thread_func(void *arg);

Explanation:
- The parameter type must be void*
- The return type must be void*
- After receiving `void* arg`, you can cast it using:
    parameters *t = (parameters *) arg;

--------------------------------------------------
4. Is main() a Thread Function?
--------------------------------------------------
- `main` is the entry point of the main thread. 
    When a program starts, the operating system automatically creates a main thread, which begins execution at `main`
- However, `main` is not a thread function and cannot be passed to `pthread_create` as a thread entry function

--------------------------------------------------
5. pthread_create Function Description
--------------------------------------------------
Function prototype:
    int pthread_create(
        pthread_t *thread,
        const pthread_attr_t *attr,
        void *(*start_routine)(void *),
        void *arg
    );

Parameter description:
- pthread_t *thread:
    Output parameter; the thread ID is written here upon successful creation
- attr:
    Thread attributes; NULL is sufficient for this project
- start_routine:
    Thread entry function
- arg:
    Pointer to the argument passed to the thread function

Return value:
- Returns 0: thread created successfully
- Returns non-zero: thread creation failed

--------------------------------------------------
6. pthread_join Function Description
--------------------------------------------------
Function prototype:
    int pthread_join(pthread_t thread, void **retval);

Parameter description:
- thread:
    The thread ID to wait for
- retval:
    Used to receive the void* pointer returned by the thread
    In this project, NULL may be used

Purpose:
- Blocks the calling thread until the specified thread finishes execution
- Used for thread synchronization and resource cleanup
*/
