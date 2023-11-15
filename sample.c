#include "thread_func.h"
#include <stdio.h>

void normalThread(void) {
    printf("Normal thread is running\n");
    printf("Thread ID = %d\n", getID());
}

void *threadWithArg(void *arg) {
    int *value = (int *)arg;
    printf("Thread with argument is running\n");
    printf("Thread ID = %d\n", getID());
    printf("Argument value = %d\n", *value);
}

int main(void) {
    initThreadLibrary();
    int p1 = 1, p2 = 2, value = 10;
    int thread1 = createThread(normalThread, p1);
    int thread2 = createThreadWithArg(threadWithArg, &value, p2);

    start();

    return 0;
}