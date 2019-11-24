#include<sys/types.h>
#include<sys/socket.h>
#include<boost/noncopyable.hpp>
#include<netinet/in.h>
#include<string.h>


/**
 * 目前只支持IPV4
*/
class InetAddr:boost::noncopyable
{
private:
    /* data */
    
    void InitFd();
public:
    int Fd_;
    int Port_;
    struct sockaddr_in inetAddr;
    InetAddr(int ,int fd =0);
    ~InetAddr();
    const struct sockaddr *getSockaddr()const {
        return (const struct sockaddr*) &inetAddr;
    };
};