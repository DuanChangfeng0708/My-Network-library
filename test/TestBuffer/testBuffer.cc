#include<iostream>
#include"Buffer.h"
#include<sys/eventfd.h>
#include<string.h>
#include<assert.h>
class testBuffer
{
private:
    /* data */
    int eventFd_;
    Buffer buffer;
public:
    void testBufferInit()
    {

        //ReadIndex=writeIndex=kprependable;
        assert(buffer.getReadableSize()==0);

        assert(buffer.getWriteableSize()==buffer.size()-Buffer::kPrependable);
    }
    void testAddDate(std::string date)
    {
        buffer.append(date.c_str(),date.size());
        //assert(buffer.getReadableSize()==date.size());
        //assert(buffer.getWriteableSize()==buffer.size()- date.size()-Buffer::kPrependable);   

    }
     std::string testGetDate(int size)
    {
        return buffer.retrieveAsString(size);
        //ReadIndex=writeIndex=kprependable;
        //不读完
        
        assert(buffer.getReadableSize()!=0);

        assert(buffer.getWriteableSize()==buffer.size()-Buffer::kPrependable);
    }
    std::string testGetAllDate()
    {
        return buffer.retrieveAllAsString();
        //ReadIndex=writeIndex=kprependable;
        assert(buffer.getReadableSize()==0);

        assert(buffer.getWriteableSize()==buffer.size()-Buffer::kPrependable);
    }
    int printBuffer();
};



int main()
{
    testBuffer t;
    t.testBufferInit();
    t.testAddDate("hello world1\n");
    t.testAddDate("hello world2\n");
    t.testAddDate("hello world3\n");
    std::cout<<t.testGetDate(6)<<"--\n";
    std::cout<<t.testGetAllDate();
}