#include"../../EventLoop.h"
#include"../../InetAddr.h"
#include"../../Buffer.h"
#include"../../Callback1.h"
#include"../../TcpServer.h"
#include"../../TcpConnection.h"
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<error.h>
#include<glog/logging.h>
#include<iostream>

// 测试通过TCP发送一张图片
class PhotoTestSever
{
private:
    /* data */
    EventLoop *loop_;
    InetAddr* ServerAddr;
    TcpServer server_;
    int photoFd;
public:
    void start(){
        server_.start(3);
    }
    PhotoTestSever(InetAddr *addr,EventLoop *loop)
    :ServerAddr(addr),
    loop_(loop),
    server_(loop,ServerAddr)
    {
        
        server_.setMessageCallback(
            std::bind(
                &PhotoTestSever::getPhoto,
                this,
                std::placeholders::_1
                ,std::placeholders::_2));
        server_.setConnectionCallback(std::bind(&PhotoTestSever::sendHello,this,std::placeholders::_1));
    };
    void sendHello(const TcpConnectionPtr& conn)
    {
        //conn->send("Hello client\n");
        photoFd=open("./photo.png",O_WRONLY|O_CREAT,0644);
        if(photoFd<0)
            perror("open");
    }
    void getPhoto(const TcpConnectionPtr& conn,Buffer& buffer)
    { 
        std::cout<<"buffer.getReadableSize(): "<<buffer.getReadableSize()<<"\n";
        std::string mess=buffer.retrieveAllAsString();
       
        if(write(photoFd,mess.c_str(),mess.size())<0)
            perror("write\n");
        
    }
};

int main(int argc,char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_alsologtostderr=true;
    FLAGS_colorlogtostderr=true;
    FLAGS_log_dir="./";
    FLAGS_max_log_size=1024;
    FLAGS_logbufsecs=0;

    EventLoop loop;
    InetAddr port(9581);
    PhotoTestSever photoTestSever(&port,&loop);
    photoTestSever.start();
    sleep(10);
    loop.loop();
}
