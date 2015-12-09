### 原子变量

Win32 API的Increment系列API
Compare and Change
C++11的atomic

### 加锁的队列
mutex: Win32的CRITICAL_SECTION，pthread_mutext
创建一个队列queue
生产者线程，获取mutex,push数据
消费者线程，获取mutex,pop数据


### 基于发送消息
线程带有邮箱mailslot，消息发送到邮箱，线程从邮箱里取数据
actor模型
