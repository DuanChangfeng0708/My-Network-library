#include"./Poller.h"
#include"./Channel.h"
#include"./EventLoop.h"
#include<poll.h>

Poller::Poller(EventLoop *loop)
:loop_(loop)
{
}
void Poller::poll(ActivityChannels *activityChannels)
{
    int nEvents=::poll(&(*(pollfds_.begin())),pollfds_.size(),-1);
    if(nEvents>0)
    {
        fillActivityChannels(nEvents,activityChannels);
    }
    else if(nEvents<0)
    {
         std::cout<<"error in Poller::poll"<<std::endl;
    }
    
}
/*
找出有活动的fd 填入channel
*/
void Poller::fillActivityChannels(int nEvents,ActivityChannels *activityChannels)
{
    for(PollFdList::const_iterator c_it=pollfds_.begin();
    c_it!=pollfds_.end();
    c_it++){
        if(c_it->revents)
        {
            ChannelMap::const_iterator ChannelMap_it=Channelmap_.find(c_it->fd);
            assert(ChannelMap_it!=Channelmap_.end());
            Channel *channel=ChannelMap_it->second;
            assert(channel->fd_==c_it->fd);
            channel->setRevents(c_it->revents);
            activityChannels->push_back(channel);
        }
    }
}

/*
如果是新的channel 将它加入到map中 //
否则将map中的channel更新内容
*/
void Poller::updateChanels(Channel * channel)
{  
    if(channel->index<0)//new one 加入map和监听集struct pollfd
    {
        assert(Channelmap_.find(channel->fd_)==Channelmap_.end());
        Channelmap_[channel->fd_]=channel;
        struct pollfd pfd;
        pfd.fd=channel->fd_;
        pfd.revents=0;
        pfd.events=static_cast<short>(channel->getEvents());
        pollfds_.push_back(pfd);
        channel->index=pollfds_.size()-1;
    }
    else
    {
        assert(Channelmap_.find(channel->fd_)!=Channelmap_.end());
        assert(Channelmap_[channel->fd_]==channel);
        Channelmap_[channel->fd_]=channel;
        int idx=channel->index;
        struct pollfd &pfd=pollfds_[idx];
        pfd.events=static_cast<short>(channel->getEvents());
        pfd.revents=0;     
        
    }
}

Poller::~Poller()
{
    
}

void Poller::removeChannel(Channel * channel)
{
    assert(loop_->isInLoopThread());
    assert(Channelmap_.find(channel->fd_)!=Channelmap_.end());
    assert(Channelmap_[channel->fd_]==channel);
    //从map里面删除
    int idx=channel->index;
    assert(idx>=0&&idx<pollfds_.size());
    assert(Channelmap_.erase(channel->fd_)==1);
    
    //从pollfdlist里面删除
    std::iter_swap(pollfds_.begin()+idx,pollfds_.end());
    pollfds_.pop_back();

}