#include"EventLoop.h"
#include"Channel.h"
#include"Poller.h"
#include"MutexLock.h"
#include"MutexLockGrard.h"
#include<stdio.h>
#include<functional>
#include<string.h>
#include<poll.h>
EventLoop *loop=NULL;
MutexLock mutex_;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
void * threadfunc(void * str)
{
    printf("son thread :%d\n",pthread_self());
    {
        MutexLockGrard lock(mutex_);
        loop=new EventLoop();
        pthread_cond_signal(&cond);
      
    }
    printf("loop Init Overed\n");
    loop->loop();
    
    /*pollfd pfd;
    pfd.fd=3;
    pfd.events=POLLIN|POLLPRI;
    pfd.revents=0;

    ::poll(&pfd,1,-1);
    printf("pollOvered Overed\n");*/
}
void printHelloWorld()
{
    printf("printHelloWorld() RUNIN thread :%d\n",pthread_self());
    printf("hello world \n");
}
int main()
{
    pthread_t thread;
    printf("father thread :%d\n",pthread_self());
    pthread_create(&thread,NULL,threadfunc,NULL);
    {
        MutexLockGrard lock(mutex_);
        while(loop==NULL)
        {
            pthread_cond_wait(&cond,mutex_.getMutex());
        }
    }

    

    printf("loop Init Overed----------------\n");
    sleep(5);
    loop->RunInLoopThread(std::bind(&printHelloWorld));
    /*int64_t a=9;
    if(write(3,&a,sizeof a)<0)
        perror("write\n");
    else
    {
        printf("write over\n");
    }
    */
    pthread_join(thread,NULL);
}