#include<boost/shared_ptr.hpp>
#include<boost/scoped_ptr.hpp>
#include<boost/utility.hpp>
#include"Callback1.h"

class Channel;
class EventLoop;
class InetAddr;
class Buffer;
class TcpConnection : boost::noncopyable,
                      public std::enable_shared_from_this<TcpConnection>
{
private:
    /* data */
    const std::string name_;
    enum StateE{Kconnecting,Kconnected};
    EventLoop* loop_;
    boost::scoped_ptr<Channel> clientChannel_;
    StateE state_;  //FIXME use atomic Variable
    const InetAddr* thisAddr_;
    const InetAddr* peerAddr_;
    ConnectionCallback  connetionCallBack_;
    MessageCallback  messageCallback_ ;
    CloseCallback closeCallback;
    
    Buffer* inputBuffer_;
    Buffer* outputBuffer_;
   
    
     
public:
    TcpConnection(const std::string name,EventLoop *loop,const InetAddr *thisAddr,const InetAddr *peerAddr);
    void ConnetEstablished();
    void setConnectionCallback(const ConnectionCallback &cb){connetionCallBack_=cb;};
    void setMessageCallback(const MessageCallback &cb){messageCallback_=cb;};
    void setcloseCallback(const CloseCallback &cb){closeCallback=cb;};
    void handleRead();
    void setstate(StateE state){state_=state;};
    ~TcpConnection();
    void handleClose();
    void handleError();
    void handleWrite();
    void ConnetionDesdroy();
    const std::string getName(){return name_;};
    void UnregisterChannel();
    /*发送数据*/
    void send(const std::string &message);
    void sendInLoop(const std::string &message);
    EventLoop * getIOloop(){return loop_;};

};

