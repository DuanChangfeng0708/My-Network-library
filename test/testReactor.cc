#include"../EventLoop.h"
#include"../Channel.h"
#include"../Poller.h"
#include<iostream>
#include<string.h>
#include<sys/timerfd.h>
using namespace std;

void readcallbackfunc()
{
    cout<<"timer readcallbackfunc \n";
}
EventLoop *loop;
int main()
{
    EventLoop m_loop;
    loop=&m_loop;

    int timerfd=::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);

    Channel channel(timerfd,loop);
    channel.setReadCallBack(readcallbackfunc);
    channel.enabelReading();

    struct itimerspec howlong;
    bzero(&howlong,sizeof howlong);
    howlong.it_value.tv_sec=5;
    ::timerfd_settime(timerfd,0,&howlong,NULL);

    loop->loop();
    ::close(timerfd);

}