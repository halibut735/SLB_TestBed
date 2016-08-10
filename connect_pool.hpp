//
//  connect_pool.hpp
//  mesa_sender
//
//  Created by 中央政治局常委 on 16/8/8.
//  Copyright © 2016年 中央政治局常委. All rights reserved.
//

#ifndef connect_pool_hpp
#define connect_pool_hpp

#include <stdio.h>
#include <netinet/in.h>
#include "stream.hpp"

class connect_pool {
public:
    connect_pool (const char ip[], short port, int connects);
    connect_pool (int thread_num);
    virtual ~connect_pool();
    void send(int index, stream_base &);
    void set_addr (const char ip[], short port);
    
private:
    struct      sockaddr_in server;
    int         create_udp(int n);
    int         max_connects;
    int*        sockets;
    char **     buf;
    
    void buf_free();
    void set_connections (int connects = 1);
    int send (int tid, char buf[], int n);
};

#endif /* connect_pool_hpp */
