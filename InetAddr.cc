#include"InetAddr.h"
#include"iostream"
#include<glog/logging.h>
InetAddr::InetAddr(int port,int fd)
    :Port_(port)
{
    if(fd==0)
        Fd_=socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
    else
        Fd_=fd;
    if(Fd_<0)
        LOG(FATAL)<<"socket(3)\n";
    bzero(&inetAddr,sizeof inetAddr);
    inetAddr.sin_port=htons(Port_);
    inetAddr.sin_family=AF_INET;
    inetAddr.sin_addr.s_addr=htonl(INADDR_ANY);
}

InetAddr::~InetAddr()
{

}