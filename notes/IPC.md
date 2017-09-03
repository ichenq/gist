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

debug mode
buffer size: 08, loop: 1000000, used 38 ms
buffer size: 12, loop: 1000000, used 34 ms
buffer size: 16, loop: 1000000, used 66 ms
buffer size: 20, loop: 1000000, used 232 ms
buffer size: 24, loop: 1000000, used 114 ms
buffer size: 32, loop: 1000000, used 164 ms

release mode
buffer size: 08, loop: 1000000, used 37 ms
buffer size: 12, loop: 1000000, used 30 ms
buffer size: 16, loop: 1000000, used 27 ms
buffer size: 20, loop: 1000000, used 96 ms
buffer size: 24, loop: 1000000, used 98 ms
buffer size: 32, loop: 1000000, used 100 ms

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>


#ifdef _WIN32
# include <Windows.h>
# include <MMSystem.h>
# pragma comment(lib, "winmm")
unsigned int getTickCount()
{
    return timeGetTime();
}
#else
unsigned int getTickCount()
{
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (ts.tv_sec * 1000000UL) + ts.tv_nsec/1000000;
}
#endif

template <int N>
void bench_string(int loop_count)
{
    std::vector<std::string> vec;
    vec.resize(loop_count);

    char szbuf[N] = {};
    memset(szbuf, 'a', N - 1);

    uint32_t start = getTickCount();
    for (int i = 0; i < loop_count; ++i)
    {
        vec[i] = szbuf;
    }
    uint32_t elapsed = getTickCount() - start;
    printf("buffer size: %02u, loop: %u, used %u ms\n", N, loop_count, elapsed);
}

int main(int argc, char* argv[])
{
#ifdef NDEBUG
    printf("release mode\n");
#else
    printf("debug mode\n");
#endif
    const size_t loop_count = 1000000; // 一百万次
    bench_string<8>(loop_count);
    bench_string<12>(loop_count);
    bench_string<16>(loop_count);
    bench_string<20>(loop_count);
    bench_string<24>(loop_count);
    bench_string<32>(loop_count);

    return 0;
}
