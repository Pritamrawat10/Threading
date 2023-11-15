#ifndef THREADLIBRARY_H
#define THREADLIBRARY_H

#include <ucontext.h>
#include <time.h>

#define MAX_THREADS 10
#define STACK_SIZE 1000
#define DEFAULT_PRIORITY 0

enum ThreadStatus {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
};

typedef struct {
    ucontext_t context;
    int id;
    enum ThreadStatus status;
    int priority;
    void (*function)(void);
    void *(*argFunction)(void *);
    void *arg;
    void *result;
    int joinID;
    int mutex;  // Simple mutex (0: unlocked, 1: locked)
} Thread;

void initThreadLibrary(void);
int createThread(void (*function)(void), int);
int createThreadWithArg(void *(*function)(void *), void *arg, int);
void yieldThread(void);
void runThread(int threadID);
void runScheduler(void);
void start(void);
void lockMutex(int threadID);
void unlockMutex(int threadID);
void setThreadPriority(int threadID, int priority);
void deleteThread(int threadID);
void sleepThread(int sec);
int getID(void);
void cleanThread(void);
void joinThread(int threadID);

#endif
