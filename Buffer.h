#include<boost/noncopyable.hpp>
#include<vector>
#include<string>
/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode
class Buffer
{
private:
    /* data */

    int readIndex;
    int writeIndex;
    std::vector<char> date_;
    void EnlargeDate(int len);
public:
    static const int kPrependable=8;
    static const int kInitSize=1024;
    int getReadableSize(){return writeIndex-readIndex;};
    int getWriteableSize(){return date_.size()-writeIndex;};
    const char* getReadHead(){return &*(date_.begin())+readIndex;};
    //从缓冲区中拿出数据
    void retrieve(int size);
    std::string retrieveAllAsString();
    std::string retrieveAsString(int size);
    //往缓冲区写数据
    void append(const void *date,int size);
    void append(const std::string date);
    //复原read段的数据
    void RecoverRead(const void *date,int size);
    char* getWriteHead(){return &*date_.begin()+writeIndex;}
    void swap(Buffer &buffer);
    int readFd(int fd);
    int size(){return date_.size();};

    Buffer(/* args */);
    ~Buffer();
};

