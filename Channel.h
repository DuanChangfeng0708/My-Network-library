#include<boost/utility.hpp>
#include<functional>

class EventLoop;
/*
    Channel 负责一个fd的事件分发，它不持有这个fd，不对它的生命周期负责。
    Channel 为不同的IO事件分发不同的回调。
    核心函数  handleEvent update@ 
*/

class Channel:boost::noncopyable
{

private:
    typedef   std::function<void()> EventCallBack;
    //typedef void(*EventCallBack)() ; 禁用  std::bind 仅仅支持std::function<T>
    /* data */
    int events_; //所有事件集
    int revents_; //它关心的事件集
   
    EventLoop *loop_;
    EventCallBack readCallback_;
    EventCallBack writeCallback_;
    EventCallBack errorCallback_;
    static const int KNoneEvent;
    static const int KReadEvent;
    static const int KWriteEvent;
    bool  isWriting_;
public:
    const int fd_;
    int index;      //  给poller使用 用于判断是否在channlemap中 
    Channel(int fd,EventLoop *loop);
    void setRevents(int );
    void enabelReading(){events_|=KReadEvent;update();};
    void setReadCallBack(EventCallBack cb){readCallback_=cb;};
    void setWriteCallBack(EventCallBack cb){writeCallback_=cb;};
    void setErrorCallBack(EventCallBack cb){errorCallback_=cb;};
    void update(); //更新所持有fd的IO事件
    void handleEvent();//Channel的核心 由loop调用 根据revents_值分别调用不同的回调
    int getEvents();
    void disableAll();
    EventLoop* getOwnerLoop(){return loop_;};
    int iskNonEvent(){return events_==KNoneEvent;};
    ~Channel();
    void UnregisterSelf();
    void enableWrite(){events_|=KWriteEvent;isWriting_=true;};
    void disableWrite(){events_^=KWriteEvent;isWriting_=false;};
    bool getWriteState(){return isWriting_;};
    void setWriteState(bool writeState){isWriting_=writeState;};
};