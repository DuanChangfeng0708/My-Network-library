#include<iostream>
#include<sys/eventfd.h>
#include<poll.h>
#include<unistd.h>
#include<pthread.h>
#include<vector>
#include"Callback1.h"
class MyEventFd
{
private:
    /* data */
    int eventfd_;
    std::vector<struct pollfd> pollfds_;
    int creatEventFd();
public:
    MyEventFd(/* args */);
    void write2Eventfd();
    void poll();
    void addpollfd();
    ~MyEventFd();
};

MyEventFd::MyEventFd(/* args */)
:eventfd_(creatEventFd())
{
   
    
}
MyEventFd::~MyEventFd()
{
}
int MyEventFd::creatEventFd()
{
    
    int fd=::eventfd(0,EFD_NONBLOCK);
    if(fd<0)
        perror("eventFd creat\n");
    else 
        return fd;
}
void MyEventFd::addpollfd()
{
   
    {
    struct pollfd pfds;
    pfds.events= POLLIN | POLLPRI;
    pfds.fd=3;
    pfds.revents=0;
    pollfds_.push_back(pfds);
    }

   
   
}
void MyEventFd::poll()
{
     int nready=::poll(&*pollfds_.begin(),pollfds_.size(),-1);
    if(nready<0)
        perror("poll\n");
    else
    {
         std::cout<<nready<<std::endl;
        int64_t c=-1;
        int readLen=::read(eventfd_,&c,sizeof c);
        std::cout<<"nready is "<< nready<<" c is "<<c<<std::endl;
    }
    std::cout<<"111112233\n";
}
void MyEventFd::write2Eventfd()
{
    int64_t a=9;
    int writenlen=::write(eventfd_,&a,sizeof a);
    if(writenlen<0)
        perror("write\n");
}

MyEventFd efd;
void *threadfunc(void *a)
{
    std::cout<<"子线程:"<<pthread_self()<<"\n";
    sleep(5);
    efd.write2Eventfd();
}
int main()
{
    std::cout<<"主线程:"<<pthread_self()<<"\n";
    pthread_t thread;
    pthread_create(&thread,NULL,threadfunc,NULL);
    efd.addpollfd();
    efd.poll();
    int aa;
    std::cin>>aa;
    return 0;
}

