#include "pthread.h" /*importing the threrad library*/
#include "stdio.h"
#include "unistd.h" /*importing POSIX Operating System API library*/
#include "string.h"
#define MEMBAR __sync_cynchronize() /*memory barrier instruction*/
#define THREAD_COUNT 8
volatile int num[THREAD_COUNT]; /*volatile prevents the compiler from applying any optimizations.*/
volatile int selecting[THREAD_COUNT];
volatile int res;

void lock_thread(int thread) {

}

