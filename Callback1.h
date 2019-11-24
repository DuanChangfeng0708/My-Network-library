
#include<memory>
#include<functional>
class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void()> TimerCallback;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
typedef std::function<void (const TcpConnectionPtr&, size_t)> HighWaterMarkCallback;

// the data has been read to (buf, len)
// char *->buffer
typedef std::function<void (const TcpConnectionPtr&,
                            Buffer &)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn);
// Timestamp
void defaultMessageCallback(const TcpConnectionPtr& conn,Buffer& buffer);