// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include "zhelpers.h"


using namespace std;

const char* MSG_QUEUE = "inproc://message.queue";


static void create_master(void* context)
{
    void* master = zmq_socket(context, ZMQ_ROUTER);
    assert(master);
    int rc = zmq_bind(master, MSG_QUEUE);
    assert(rc == 0);
    while (true)
    {
        char* from = s_recv(master);
        char* to = s_recv(master);
        char* msg = s_recv(master);
        cout << from << " --> " << to << " : " << msg << endl;
        s_sendmore(master, to);
        s_sendmore(master, from);
        s_send(master, msg);
        free(from);
        free(to);
        free(msg);
    }
}

static void create_actor1(void* context)
{
    void* actor1 = zmq_socket(context, ZMQ_DEALER);
    assert(actor1);
    int rc = zmq_setsockopt(actor1, ZMQ_IDENTITY, "actor1", 6);
    assert(rc == 0);
    rc = zmq_connect(actor1, MSG_QUEUE);
    assert(rc == 0);
    int counter = 1;
    while (true)
    {
        char buffer[255];
        sprintf_s(buffer, _countof(buffer), "hello, actor2 [%d]", counter);
        s_sendmore(actor1, "actor2");
        s_send(actor1, buffer);
        counter++;

        char* from = s_recv(actor1);
        char* msg = s_recv(actor1);
        //cout << msg << "\t" << "From:" << from << endl;
        free(from);
        free(msg);

        this_thread::sleep_for(chrono::seconds(1));
    }
}

static void create_actor2(void* context)
{
    void* actor2 = zmq_socket(context, ZMQ_DEALER);
    int rc = zmq_setsockopt(actor2, ZMQ_IDENTITY, "actor2", 6);
    assert(rc == 0);
    rc = zmq_connect(actor2, MSG_QUEUE);
    assert(rc == 0);
    int counter = 0;
    while (true)
    {
        char* from = s_recv(actor2);
        char* msg = s_recv(actor2);
        //cout << msg << "\t" << "From:" << from << endl;
        counter++;
        char buffer[255];
        sprintf_s(buffer, _countof(buffer), "Nice to meet you [%d]", counter);
        s_sendmore(actor2, from);
        s_send(actor2, buffer);
        free(from);
        free(msg);

        this_thread::sleep_for(chrono::seconds(1));
    }
}


int main(int argc, char* argv[])
{
    void* context = zmq_ctx_new();

    thread master(create_master, context);
    thread actor1(create_actor1, context);
    thread actor2(create_actor2, context);

    master.join();
    actor1.join();
    actor2.join();

    zmq_ctx_destroy(context);

    return 0;
}