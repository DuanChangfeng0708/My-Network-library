#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<glog/logging.h>
int main(int argc,char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_alsologtostderr=true;
    FLAGS_colorlogtostderr=true;
    FLAGS_log_dir="./";
    FLAGS_max_log_size=1024;
    FLAGS_logbufsecs=0;

    int clientfd=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    bzero(&addr,sizeof addr);

    addr.sin_family=AF_INET;
    addr.sin_port=htons(9581);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);
    if(connect(clientfd,(struct sockaddr *)&addr,sizeof(addr))<0)
        perror("connet\n");
    //---------------------------
    //|图片请替换为你自己想要传输图片|
    //---------------------------
    int photofd=open("/home/dk/图片/123.png",O_RDONLY);
    if(photofd<0)
    {
       
        perror("open\n");
         exit(0);
    }

    char buf[1024]={0};
    int readlen=0;
    int count=0;
    while(readlen=read(photofd,buf,sizeof(buf)))
    {
        if(readlen<0)
            perror("read\n");
        int writelen=write(clientfd,buf,readlen);
        if(writelen<0||writelen!=readlen)
            perror("write\n");
        printf("writelen %d count %d\n",writelen,++count);
    }
    printf("发送完成\n");
    sleep(5);
    
}