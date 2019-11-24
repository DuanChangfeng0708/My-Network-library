#include<boost/noncopyable.hpp>
#include<functional>

class EventLoop;
class Channel;
class InetAddr;
/**
 * 这是一个内部类，供Tcpserver使用 负责新客户端的连接
*/
class Acceptor
{
    typedef std::function<void(const InetAddr*)> NewConnetionCallback;
private:
    /* data */
    EventLoop *loop_;
    int linstenFd_;
    Channel *linstenChannel_;
    void handleRead();
    bool islistenning_;
    NewConnetionCallback newConnetionCallback_;
public:
    Acceptor(EventLoop *loop,InetAddr *addr);
    ~Acceptor();
    void Listen();
    void setNewConnetionCallback(const NewConnetionCallback &cb){newConnetionCallback_=cb;};
    
};

