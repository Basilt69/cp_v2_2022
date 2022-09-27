#include "pthread.h" /*importing the threrad library*/
#include "stdio.h"
#include "unistd.h" /*importing POSIX Operating System API library*/
#include "string.h"
#define MEMBAR __sync_synchronize() /*memory barrier instruction*/
#define THREAD_COUNT 8
volatile int num[THREAD_COUNT]; /*volatile prevents the compiler from applying any optimizations.*/
volatile int selecting[THREAD_COUNT];
volatile int res;

void lock_thread(int thread) {
    // Before getting the ticket number
    //selecting variable is set true
    selecting[thread] = 1;
    MEMBAR;
    //Memory barrier applied
    int max_num = 0;
    //Finding Maximum ticket value among current threads
    for (int i=0;i<THREAD_COUNT;i++)
    {
        int ticket = num[i];
        max_num =ticket>max_num?ticket:max_num;
    }
    //Alloting new ticket value as maximum+1
    num[thread] = max_num+1;
    MEMBAR;
    selecting[thread] = 0;
    MEMBAR;
    //ENTRY Secion starts
    for (int other=0;other<THREAD_COUNT;++other)
    {
        //Applying the bakery algorithm conditions
        while (selecting[other])
        {

        }
        MEMBAR;
        while (num[other] != 0 && (num[other] < num[thread] || (num[other] == num[thread] && other < thread)))
        {}
    }
}
//EXIT Section
void unlock_thread(int thread)
{
    MEMBAR;
    num[thread]=0;
}
//CRITICAL Section
void use_res(int thread)
{
    if (res !=0)
    {
        printf("The resource was acquired by %d, but is still in-use by %d!\n", thread, res);
    }
    res=thread;
    printf("%d is using resource ...\n",thread);

    MEMBAR;
    sleep(2);
    res=0;
}

//Simplified function to show the implementation
void *thread_body(void *arg)
{
    long thread = (long)arg;
    lock_thread(thread);
    use_res(thread);
    unlock_thread(thread);
    return NULL;
}

int main(int argc, char **argv)
{

}

