# BSD Socket


### BSD API
https://en.wikipedia.org/wiki/Berkeley_sockets

* socket()
* bind()
* listen()
* accept()
* connect()
* recv() / send()
* shutdown()
* close()


### BSD代码示例
一段用BSD Socket API写的server代码：

~~~ cpp
#include <stdint.h>
#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")


void server(const char* host, uint16_t port)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);

    SOCKET acceptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(acceptor, (sockaddr*)&addr, sizeof(addr));
    listen(acceptor, SOMAXCONN);
    printf("server started\n");
    for (;;)
    {
        char buffer[1024];
        SOCKET fd = accept(acceptor, NULL, NULL); //阻塞直到客户端连接

        //这里可以使用单独的线程
        int size = recv(fd, buffer, 1024, 0);
        send(fd, buffer, size, 0);
        closesocket(fd);
    }
}

int main(int argc, char* argv[])
{
    const char* host = "127.0.0.1";
    uint16_t port = 10086;

    WSADATA data;
    WSAStartup(2, &data);

    server(host, port);

    return 0;
}
~~~

一段用BSD Socket API写的client代码：
~~~ cpp
#include <stdint.h>
#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")

void client(const char* host, uint16_t port)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);
    SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(fd, (sockaddr*)&addr, sizeof(addr));

    // send a message
    const char* msg = "a quick brown fox jumps over the lazy dog";
    send(fd, msg, strlen(msg), 0);

    // recv and print
    char buffer[1024] = {};
    int size = recv(fd, buffer, 1024, 0);
    printf("%s\n", buffer);

    closesocket(fd);
}

int main(int argc, char* argv[])
{
    const char* host = "127.0.0.1";
    uint16_t port = 10086;

    WSADATA data;
    WSAStartup(2, &data);

    client(host, port);

    return 0;
}
~~~

### BSD API和异步I/O

read/write操作都是同步API，process需要等待OS完成I/O传输后才能够继续往下执行，这种方式容易理解，编程实现也简单。但是I/O操作相比计算操作通常非常缓慢，让CPU一直处于等待阶段不利于整个系统的吞吐。

异步I/O就是process执行I/O操作的时候会立刻得到OS的返回，但OS此刻还没有完成相应的I/O传输，而是在I/O传输完成后通过特定的通知机制告诉process I/O已经完成，你可以做跟这个I/O后续的事情了。

这个通知机制在不同的平台下有不同的模型，通常我们只关心Windows和Linux平台，那必须要了解的是：

* select()，跨平台最好的I/O复用模型
* I/O Completion Port，Windows下性能最好的I/O复用模型
* epoll，Linux下性能最好的I/O复用模型

开源的网络库，无论是用C语言编写的libevent, libuv，还是C++编写的asio，其核心都是对这几个模型的封装。


# IO复用

### select模型
select的方式是将socket设置为no-blocking模式，然后在一个循环里用select()来做轮询。

select的函数签名是这样:

~~~ cpp
int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);
~~~

三个fd_set监听的事件类型如下：

readfds:
* If listen has been called and a connection is pending, accept will succeed.
* Data is available for reading (includes OOB data if SO_OOBINLINE is enabled).
* Connection has been closed/reset/terminated.

writefds:
* If processing a connect call (nonblocking), connection has succeeded.
* Data can be sent.

exceptfds:
* If processing a connect call (nonblocking), connection attempt failed.
* OOB data is available for reading (only if SO_OOBINLINE is disabled).

更多的细节，请参考下面的连接
[select(2): synchronous I/O multiplexing](http://linux.die.net/man/2/select)
[select function](https://msdn.microsoft.com/en-us/library/windows/desktop/ms740141(v=vs.85).aspx)

select的优点
* 主流操作系统平台都支持它(Windows, Linux, OS X, FreeBSD, iOS, Android)，编写跨平台的网络代码时候用select非常合适

* API支持超时处理，有的库(如[RCF这个RPC框架](http://www.deltavsoft.com/))使用select来做connect超时判断

它的缺点
*  轮询非常低效，每次都要轮询整个socket数组
*  万恶1024限制（fd_set的大小），不适合用在服务器跟客户端的连接处理


下面是一个如何使用select的示例代码:
~~~ cpp

void server(const char* host, uint16_t port)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);

    SOCKET acceptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(acceptor, (sockaddr*)&addr, sizeof(addr));
    listen(acceptor, SOMAXCONN);

    // set to non-blocking mode 
    ULONG nonblock = 1;
    ioctlsocket(acceptor, FIONBIO, &nonblock);
    printf("server started\n");

    for (;;)
    {
        int nready;
        fd_set readset;
        FD_SET(acceptor, &readset);
        readset.fd_count = 1; // only acceptor
        struct timeval timeout = { 0, 100 * 1000 }; // 10 ms timeout
        SOCKET fd = accept(acceptor, NULL, NULL);
        nready = select(0, &readset, NULL, NULL, &timeout);
        if (nready == SOCKET_ERROR)
        {
            // error handling
        }
        else if (nready == 0)
        {
            // time out
        }
        else 
        {
            //遍历所有socket, FD_ISSET
        }
    }
}
~~~

这里是我用select写就的一个简易echo server
https://github.com/ichenq/WinsockTut/tree/master/src/select

### I/O Completion Port

IOCP是一个[Proactor模式](https://en.wikipedia.org/wiki/Proactor_pattern)的I/O模型

https://github.com/ichenq/WinsockTut/tree/master/src/iocp_server