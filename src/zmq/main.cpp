#include <zmq.h>
#include <thread>
#include <chrono>

using namespace std;

const char* ADDRESS = "tcp://127.0.0.1:12315";

void worker(void* context)
{
    void* dealer = zmq_socket(context, ZMQ_DEALER);
    zmq_setsockopt(dealer, ZMQ_IDENTITY, "1001", 4);
    zmq_connect(dealer, ADDRESS);
    for (int i = 0; i < 5; i++)
    {
        zmq_send(dealer, "hello", 5, 0);
        this_thread::sleep_for(chrono::seconds(3));
    }
}

void broker(void* context)
{
    void* broker = zmq_socket(context, ZMQ_ROUTER);
    zmq_bind(broker, ADDRESS);
    while (true)
    {
        zmq_msg_t msg;
        zmq_msg_init(&msg);
        int r = zmq_msg_recv(&msg, broker, 0);
        if (r < 0)
            break;
        zmq_msg_close(&msg);
    }
}

int main(int argc, char* argv[])
{
    void* context = zmq_ctx_new();
    std::thread thr1(worker, context);
    std::thread thr2(broker, context);
    thr1.join();
    thr2.join();
    zmq_ctx_shutdown(context);
    return 0;
}