#include<boost/utility.hpp>
#include<vector>
#include<map>

class Channel;
class EventLoop;
/*
    此类负责socket的监听  由Eventloop调用，只能在IO线程使用 所以不需要加锁
*/
class Poller
{
    
private:
    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int,Channel*> ChannelMap;

    EventLoop* loop_;
    PollFdList pollfds_;
    ChannelMap Channelmap_;
    
    /* data */
public:
    typedef std::vector<Channel*> ActivityChannels;
    
    Poller(EventLoop *);
    void poll(ActivityChannels *); //贡eventloop使用，返回活跃的channel集合 参数为传出参数
    void fillActivityChannels(int,ActivityChannels*);
    void updateChanels(Channel *); //供eventloop使用
    ~Poller();
    void removeChannel(Channel *channel);
};