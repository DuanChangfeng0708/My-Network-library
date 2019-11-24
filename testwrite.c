#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{
    int fd=open("testwrite.txt",O_WRONLY|O_CREAT);
    int a=1;
    int writelen=write(fd,&a,sizeof a);
    if(writelen<0)
        perror("write\n");
    close(fd);
     int fd2=open("testwrite.txt",O_RDONLY|O_CREAT);
    int c=-1;
    int readbuf=read(fd2,&c,sizeof c*10);
    if(readbuf<0)
        perror("read\n");
    printf("readbuf %d c=%d\n",readbuf,c);
    return 0;
}
