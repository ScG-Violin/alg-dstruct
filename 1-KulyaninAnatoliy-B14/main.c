#pragma warning(disable : 4996)
#include "memallocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define MAX_ALLOCATING_SIZE 1000
#define MAX_ITERATIONS 100
// Use to init the clock
#define TIMER_INIT \
LARGE_INTEGER frequency; \
LARGE_INTEGER start; \
LARGE_INTEGER end; \
double elapsedTime; \
QueryPerformanceFrequency(&frequency);
//Use to start the timer
#define TIMER_START QueryPerformanceCounter(&start);
// Use to stop the timer/
#define TIMER_STOP \
QueryPerformanceCounter(&end); \
elapsedTime=(double)(end.QuadPart-start.QuadPart)/frequency.QuadPart;

double mallocTest(int size) {
    TIMER_INIT
        void* arr[MAX_ITERATIONS] = { NULL };
    int last_i = 0;
    TIMER_START
        for (int i = 0; i < MAX_ITERATIONS; i++) {
            if (rand() % 3 != 0) {
                arr[i] = malloc(size);
                last_i = i;
            }
            else if (arr[last_i] != NULL) {
                free(arr[last_i]);
                arr[last_i] = NULL;
            }
        }
    TIMER_STOP
        for (int i = 0; i < MAX_ITERATIONS; i++) {
            free(arr[i]);
        }
    return elapsedTime;

}

double memallocTest(int size) {
    TIMER_INIT
        void* arr[MAX_ITERATIONS] = { NULL };
    int last_i = 0;
    TIMER_START
        for (int i = 0; i < MAX_ITERATIONS; i++) {
            if (rand() % 3 != 0) {
                arr[i] = memalloc(size);
                last_i = i;
            }
            else if (arr[last_i] != NULL) {
                memfree(arr[last_i]);
                arr[last_i] = NULL;
            }
        }
    TIMER_STOP
        for (int i = 0; i < MAX_ITERATIONS; i++) {
            memfree(arr[i]);
        }
    return elapsedTime;
}

int main(void) {
    double time[2];
    FILE* f;
    if ((f = fopen("cmp.csv", "w")) == NULL)
        printf("The file 'cmp' was not opened\n");
    int memorySize = MAX_ITERATIONS * (MAX_ALLOCATING_SIZE + memgetblocksize());
    void* ptr = malloc(memorySize);
    if (ptr == NULL) {
        printf("Error allocating memory\n");
        exit(1);
    }
    int init = meminit(ptr, memorySize);
    if (init) {
        printf("Error initialize allocating system\n");
        exit(1);
    }
    if (f) {
        fprintf(f, "malloc;memalloc\n");
        for (int i = 10; i < MAX_ALLOCATING_SIZE; i += 10) {
            time[0] = mallocTest(i);
            time[1] = memallocTest(i);
            fprintf(f, "%lf;%lf\n", time[0], time[1]);
        }
        fclose(f);
    }
    memdone();
    free(ptr);
    return 0;
}