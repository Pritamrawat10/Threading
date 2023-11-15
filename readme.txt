thread library is a simple C Library which help to implement multi threading 
in c language , you can create thread with argument as well withour argument . You can set the priority of thread if you want to .


Thread Structure:

The program defines a Thread structure to represent individual threads. This structure includes information such as thread ID, status, priority, function pointers, context, and a mutex for synchronization.
Initialization:

The initializeThreadLibrary function initializes an array of Thread structures, setting their status to FINISHED and initializing mutexes.
Thread Creation:

createThread and createThreadWithArg functions create new threads. They allocate stack space, set up the thread context, and assign the provided function or function with arguments. Threads are identified by their unique IDs.
Thread Scheduling:

The program implements a simple thread scheduler in the runScheduler function. It selects the next ready thread with the highest priority and runs it. The scheduler continues until all threads finish.
Thread Yielding:

yieldToHighestPriorityThread yields the current thread in favor of the highest-priority ready thread.
Thread Priority:

setThreadPriority allows setting the priority of a specific thread.
Thread Running and Suspension:

runThread marks a thread as ready, and suspendThread and resumeThread change a thread's status to blocked or ready, respectively.
Thread Joining:

joinThread blocks the current thread until the specified thread finishes.
Mutex Locking and Unlocking:

lockMutex and unlockMutex provide basic mutex functionality for thread synchronization.
Thread Deletion and Cleanup:

deleteThread frees the stack space of a finished thread, and cleanThread deletes all active threads.
Thread Sleeping:

sleepThread puts the current thread to sleep for a specified number of seconds.
Other Utility Functions:

getID returns the ID of the current thread.
start initiates the thread scheduler.


