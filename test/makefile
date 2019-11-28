CXX = g++
TARGET = testTcpserver
C_FLAGS += -g 
LIB_FLAGS = -lglog -pthread

vpath %.cc ../
vpath %.h ../

Src = MutexLock.o MutexLockGrard.o testTcpServer.o Channel.o EventLoop.o Poller.o \
TcpServer.o Acceptor.o InetAddr.o TcpConnection.o EventLoopThread.o EventLoopThreadPool.o Buffer.o
# 注释
all: $(TARGET)
testTcpserver: $(Src)
	$(CXX) $(C_FLAGS) -o $@ $^ $(LIB_FLAGS) 

#MutexLockGrard.o:MutexLockGrard.h MutexLockGrard.cc
#testTcpServer.o:testTcpServer.cc
#MutexLock.o:MutexLock.h
#Acceptor.o:Acceptor.h
#InetAddr.o:InetAddr.h
#Channel.o:Channel.h
#EventLoop.o:EventLoop.h
#Poller.o:Poller.h
#TcpServer.o:TcpServer.h 
.cc.o:
	$(CXX) -c $(C_FLAGS) $<


	
clean:
	-rm -f *.o $(TARGET)
