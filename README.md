# My-Network-library
这是一个自己写的C++网络库，主要特征如下
线程安全，多线程网络库
不考虑平台移植性，不跨平台，只支持Linux
网络通信协议只支持TCP，不支持UDP
只支持IPv4，暂不支持IPv6
只支持一种使用模式，即非阻塞IO+one event loop per thread，不支持阻塞IO
API简单易用，源码全部开源
暂时用poll作为IO multiplexing，epoll将在下一版更新使用
使用google的glog作为日志库
目前只有服务端程序，用户可以自己编写TCP客户端或直接用nc命令测试

设计思路及框架结构见博客https://blog.csdn.net/qq_40832456/category_9356354.html

安装方式：
本网络库依赖于Boost库和glog库，使用之前请自行先安装这两个库
Boost安装方式：sudo apt-get install libboost-dev libboost-test-dev
glog安装方式：Git clone https://github.com/google/glog.git

如何在自己项目中使用本网络库？
只需在将本库目录符号链接到/usr/include/下即可
