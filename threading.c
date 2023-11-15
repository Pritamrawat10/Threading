#include "thread_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>


Thread threads[MAX_THREADS];
int currentThread = -1;

void initializeThreadLibrary(void) {
    for (int i = 0; i < MAX_THREADS; ++i) {
        threads[i].status = FINISHED;
        threads[i].mutex = 0;  // unlocked
        threads[i].priority = DEFAULT_PRIORITY;
    }
}

int createThread(void (*function)(void), int pri) {
      for (int i = 0; i < MAX_THREADS; ++i) {
        if (threads[i].status == FINISHED) {
            getcontext(&threads[i].context);
            threads[i].context.uc_stack.ss_sp = malloc(STACK_SIZE);
            threads[i].context.uc_stack.ss_size = STACK_SIZE;
            threads[i].context.uc_link = 0;
            threads[i].id = i;
            threads[i].status = READY;
            threads[i].priority = pri;
            threads[i].function = function;

            makecontext(&threads[i].context, function, 0);

    // returning id
            return i; 
    }
}
return -1; 
}

int createThreadWithArg(void *(*function)(void *), void *arg, int pri) {
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (threads[i].status == FINISHED) {
            getcontext(&threads[i].context);
            threads[i].context.uc_stack.ss_sp = malloc(STACK_SIZE);
            threads[i].context.uc_stack.ss_size = STACK_SIZE;
            threads[i].context.uc_link = 0;
            threads[i].id = i;
            threads[i].status = READY;
            threads[i].priority = pri;
            threads[i].argFunction = function;
            threads[i].arg = arg;

            makecontext(&threads[i].context, (void (*)(void))function, 1, arg);
        
            return i;  // Return thread ID
        }

        }
return -1; 
}



void yieldToHighestPriorityThread(void) {
   int highestPriority = INT_MIN;
    int prevThread = currentThread;
    int nextThread = -1;

    for (int i = 0; i < MAX_THREADS; ++i) {
        if (i != currentThread && threads[i].status == READY && threads[i].priority > highestPriority) {
                highestPriority = threads[i].priority;
                nextThread = i;
            }
    }

    currentThread = nextThread;

    if (prevThread != -1 && currentThread != -1 && prevThread != currentThread) {
        threads[prevThread].status = READY;
        swapcontext(&threads[prevThread].context, &threads[currentThread].context);
    }
}

void setThreadPriority(int threadID, int priority) {
    if (threadID >= 0 && threadID < MAX_THREADS) {
        threads[threadID].priority = priority;
    }
}


void runThread(int threadID) {
    threads[threadID].status = READY;
    yieldThread();
}

void runScheduler(void) {
    while (1) {
          int highestPriority = INT_MIN;
        int nextThread = -1;

        for (int i = 0; i < MAX_THREADS; ++i) {
            if (threads[i].status == READY && threads[i].priority > highestPriority) {
                highestPriority = threads[i].priority;
                nextThread = i;
            }
        }

        if (nextThread != -1) {
            currentThread = nextThread;
            threads[currentThread].status = RUNNING;

            if (threads[currentThread].argFunction != NULL) {
                threads[currentThread].result = threads[currentThread].argFunction(threads[currentThread].arg);
            } else {
                threads[currentThread].function();
            }
            threads[currentThread].status = FINISHED;
        }

        // Check for all threads finished
        int allFinished = 1;
        for (int i = 0; i < MAX_THREADS; ++i) {
            if (threads[i].status != FINISHED) {
                allFinished = 0;
                break;
            }
        }

        if (allFinished) {
            break;  // Exit the loop if all threads are finished
        }
    }
}



void cleanThread(void) {
    for (int i = 0; i < MAX_THREADS; ++i) {
        if (threads[i].status != FINISHED) {
            deleteThread(i);
        }
    }
}

void start(void) {
    runScheduler();
}

void suspendThread(int threadID) {
    threads[threadID].status = BLOCKED;
    yieldThread();
}

void resumeThread(int threadID) {
    threads[threadID].status = READY;
    yieldThread();
}

void deleteThread(int threadID) {
    free(threads[threadID].context.uc_stack.ss_sp);
    threads[threadID].status = FINISHED;
}

void sleepThread(int sec) {
    threads[currentThread].status = BLOCKED;
    threads[currentThread].result = NULL;  // Reset result
    sleep(sec);
    threads[currentThread].status = READY;
    yieldThread();
}

int getID(void) {
    if (currentThread >= 0 && currentThread < MAX_THREADS) {
        return threads[currentThread].id;
    }
    return -1;  // Invalid ID
}


void joinThread(int threadID) {
    if (threadID >= 0 && threadID < MAX_THREADS) {
        threads[currentThread].status = BLOCKED;
        threads[currentThread].joinID = threadID;
        yieldThread();  // Switch to another thread
    }
}

void lockMutex(int threadID) {
    if (threadID >= 0 && threadID < MAX_THREADS) {
        while (threads[threadID].mutex == 1) {
            yieldThread();  // Wait until the mutex is unlocked
        }
        threads[threadID].mutex = 1;  // Lock the mutex
    }
}

void unlockMutex(int threadID) {
    if (threadID >= 0 && threadID < MAX_THREADS) {
        threads[threadID].mutex = 0;  // Unlock the mutex
    }
}
