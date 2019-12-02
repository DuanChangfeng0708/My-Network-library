#include"Buffer.h"
#include<assert.h>
#include<stdio.h>
#include<iostream>
#include<sys/uio.h>
#include<glog/logging.h>
Buffer::Buffer()
:readIndex(kPrependable),
writeIndex(kPrependable)
{
    date_.resize(kPrependable+kInitSize);
}

void Buffer::retrieve(int size)
{
    if(size<getReadableSize())
        readIndex+=size;
    else
    {
        readIndex=kPrependable;
        writeIndex=kPrependable;
    }
    
}
void Buffer::append(const void *date,int len)
{
    const char *date2=(const  char *)date;
    if(len>getWriteableSize())
    {
        EnlargeDate(len);
    }
    std::copy(date2,date2+len,getWriteHead());
    writeIndex+=len;
    
}
void Buffer::append(const std::string date)
{
    int len=date.size() ;
    if(len>getWriteableSize())
        EnlargeDate(len);
    else
    {
        std::copy(date.begin(),date.end(),
        getWriteHead());
        writeIndex+=len;
    }
    
}

void Buffer::EnlargeDate(int len)
{
    if(len<readIndex-kPrependable)//内部腾挪
    {
        std::vector<char> temp(date_.begin()+readIndex,date_.begin()+writeIndex);
        date_.clear();
        std::copy(temp.begin(),temp.end(),date_.begin()+kPrependable);
        readIndex=kPrependable;
        writeIndex=readIndex+temp.size();
    }
    else                        //开辟新空间
        date_.resize(date_.size()+len);
    
}

void Buffer::RecoverRead(const void *date,int len)
{
    assert(len+kPrependable<readIndex);
    
    readIndex-=len;
    const char *temp=static_cast<const char *>(date);
    std::copy(temp,temp+len,date_.begin()+readIndex);
}

void Buffer::swap(Buffer & buffer)
{
    this->date_.resize(buffer.date_.size());
    this->readIndex=buffer.readIndex;
    this->writeIndex=buffer.writeIndex;
    this->date_.swap(buffer.date_);

}

int Buffer::readFd(int fd)
{
    char buf[65535];
    iovec ev[2];

    ev[0].iov_base=&*(date_.begin())+writeIndex;
    ev[0].iov_len=getWriteableSize();
    ev[1].iov_base=buf;
    ev[1].iov_len=65535;

    int EvCount=getWriteableSize()>sizeof buf?1:2;
    int nread=readv(fd,ev,EvCount);
    if(nread<0)
    {
        LOG(FATAL)<<"readv\n";
    }
    else if(nread<getWriteableSize())
    {
        writeIndex+=nread;
    }
    else
    {
        int restDate=nread-getWriteableSize();
        writeIndex=date_.size();
        append(buf,restDate);
    }
    return nread;
}

std::string Buffer::retrieveAllAsString()
{
    std::string temp(getReadHead(),getReadableSize());
    retrieve(getReadableSize());
    
    return temp;
}
std::string Buffer::retrieveAsString(int size)
{
    std::string temp(getReadHead(),size);
    retrieve(size);
    return temp;
}
Buffer::~Buffer()
{
    
}