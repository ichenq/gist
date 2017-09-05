// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <zmq.h>
#include <zmq_utils.h>


#define printError()    printf("%s[%d]: %d: %s\n", __FILE__, __LINE__, \
                            zmq_errno(), zmq_strerror(zmq_errno()));

using namespace std;

static char SERVER_PUBLIC_KEY[41] = "Gz3k/6%M-)}==5&u8K!33Y*?qN(nfOsz5r6g)J>-";
static char SERVER_SECRET_KEY[41] = "VC}05P&jw@4b+eBB(8z=TZ.hUw+4Anoez?>7jcZe";

static const char* SERVER_ADDR = "tcp://127.0.0.1:10086";

static void start_server(void* context)
{
    uint8_t public_key[32];
    uint8_t secret_key[32];
    zmq_z85_decode(public_key, SERVER_PUBLIC_KEY);
    zmq_z85_decode(secret_key, SERVER_SECRET_KEY);

    void* server = zmq_socket(context, ZMQ_REP);
    int rc = zmq_setsockopt(server, ZMQ_CURVE_SECRETKEY, secret_key, 32);
    assert(rc == 0);
    rc = zmq_setsockopt(server, ZMQ_CURVE_PUBLICKEY, public_key, 32);
    assert(rc == 0);

    int curve_server = 1;
    rc = zmq_setsockopt(server, ZMQ_CURVE_SERVER, &curve_server, sizeof(curve_server));
    assert(rc == 0);

    rc = zmq_bind(server, SERVER_ADDR);
    assert(rc == 0);
    uint32_t msg_count = 0;
    while (true)
    {
        zmq_msg_t msg;
        zmq_msg_init(&msg);
        rc = zmq_msg_recv(&msg, server, 0);
        if (rc == -1)
        {
            printError();
            break;
        }
        else
        {
            const char* str = (const char*)zmq_msg_data(&msg);
            size_t size = zmq_msg_size(&msg);
            string s(str, size);
            cout << msg_count++ << ": " << s << endl;
            zmq_send(server, s.data(), s.size(), 0);
            zmq_msg_close(&msg);
        }
    }
    zmq_close(server);
}


void start_client(void* context)
{
    char public_txt[41];
    char secret_txt[41];
    int rc = zmq_curve_keypair(public_txt, secret_txt);
    assert(rc == 0);

    byte public_key[32] = { 0 };
    byte secret_key[32] = { 0 };
    zmq_z85_decode(public_key, public_txt);
    zmq_z85_decode(secret_key, secret_txt);

    void* client = zmq_socket(context, ZMQ_REQ);
    rc = zmq_setsockopt(client, ZMQ_CURVE_SECRETKEY, secret_key, 32);
    assert(rc == 0);
    rc = zmq_setsockopt(client, ZMQ_CURVE_PUBLICKEY, public_key, 32);
    assert(rc == 0);
    rc = zmq_setsockopt(client, ZMQ_CURVE_SERVERKEY, SERVER_PUBLIC_KEY, 40);
    assert(rc == 0);

    rc = zmq_connect(client, SERVER_ADDR);
    assert(rc == 0);

    string s = "hello, kitty";
    rc = zmq_send(client, s.data(), s.size(), 0);
    assert(rc > 0);
    while (true)
    {
        char buffer[1024];
        rc = zmq_recv(client, buffer, 1024, 0);
        if (rc == -1)
        {
            printError();
        }
        else
        {
            this_thread::sleep_for(chrono::milliseconds(300));
            zmq_send(client, buffer, rc, 0);
        }
    }
    zmq_close(client);
}

int main(int argc, char* argv[])
{
    void* context = zmq_ctx_new();

    thread thrd_server(start_server, context);
    thread thrd_client(start_client, context);
    thrd_server.join();
    thrd_client.join();

    zmq_ctx_destroy(context);
    return 0;
}