client发起REQUEST和参数
server返回RESPONSE和返回内容


一次通信内容抽象来看如同一个函数调用：
比如登录：
bool SignIn(string account, string password);

REQUEST就是SignIn，携带的参数就是account和password
RESPONSE也是SignIn，返回bool值，表示登录成功或者失败

TCP套接字

阻塞和非阻塞


基于消息的通信方式
------

这种方式是把REQUEST和RESPONSE抽象成了消息(CMessage)

异步的处理方式，消息的接收和发送处于不同的地方。

如星辰传说的做法：
还是登录：

客户端的Request:
~~~ cpp
CMessage msg(MSG_C2S_SIGNIN);
msg.Add(account);
msg.Add(password);
msg.Send();
~~~

服务器收到这条消息后做处理，然后返回：
~~~ cpp
CMessage msg(MSG_S2C_SIGNIN);
msg.Add(result); // 登录结果
msg.SendToClient();
~~~

从TCP包的角度来看：

[长度][消息头][消息内容]


基于RPC的通信方式
------

RPC是在消息上的更高层次抽象

服务器提供这个RPC接口
~~~ cpp
bool SignIn(string account, string password);
~~~


客户端调用:
~~~ cpp
bool result = SignIn('name', '11111');
~~~
SignIn内部，将SignIn这个函数名称及其参数account和password封装，
发送给服务器，并开启一个计时器，等待服务器返回，超时则抛出异常


Server端，收到客户端的消息后，解码出SignIn和account、password参数，
查询注册的接口是否有较SignIn的函数，如果有则传递参数调用它，并将返回结果通过TCP包传递回客户端



服务器和服务器使用RPC是较好的方式
机房直接的网络通信环境相对客户端较好，通常不会有太多等待
