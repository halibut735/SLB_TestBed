//
//  connect_pool.cpp
//  mesa_sender
//
//  Created by 中央政治局常委 on 16/8/8.
//  Copyright © 2016年 中央政治局常委. All rights reserved.
//

#include "connect_pool.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <assert.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>

connect_pool::~connect_pool() {
    buf_free();
    for (int i = 0; i < max_connects; ++ i) {
        close(sockets[i]);
    }
}

int connect_pool::create_udp(int connects) {
    if (sockets) {
        free(sockets);
        sockets = NULL;
    }
    sockets = (int *) malloc(sizeof(int) * connects);
    for (int i = 0; i < connects; ++ i) {
        sockets[i] = socket(AF_INET, SOCK_DGRAM,0);
        assert(sockets[i]);
    }
    return connects;
}


void connect_pool::send(int index, stream_base &pkts) {
    unsigned int is_end = pkts.magic;
    if (is_end & pkts.END)
        pkts.magic ^= pkts.END;
    while (pkts.len) {
        if (pkts.len > 1400) {
            stream_base each(pkts.id, 1400, pkts.magic, pkts.offset);
            memcpy(buf[index], (void *) &each, sizeof(stream_base));
            send(index, buf[index], 1400 + sizeof(stream_base));
            if (pkts.magic & pkts.START)
                pkts.magic ^= pkts.START;
            pkts.len -= 1400;
        } else {
            if (is_end & pkts.END)
                pkts.magic |= pkts.END;
            stream_base each(pkts.id, pkts.len, pkts.magic, pkts.offset);
            memcpy(buf[index], (void *) &each, sizeof(stream_base));
            send(index, buf[index], pkts.len + sizeof(stream_base));
            pkts.len = 0;
        }
    }
}


int connect_pool::send(int tid, char * buf, int n) {
    return (int) sendto(sockets[tid], buf, n, 0, (struct sockaddr *)&server, sizeof(server));
}

connect_pool::connect_pool(int thread_num):sockets(NULL), max_connects(thread_num), buf(NULL) {
    if (thread_num)
        set_connections(thread_num);
}

void connect_pool::set_addr (const char ip[], short port) {
    struct hostent *he;
    if ((he = gethostbyname(ip)) == NULL) {
        printf("gethostbyname()error\n");
        exit(1);
    }
    
    bzero(&server,sizeof(server));
    
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = *((struct in_addr *)he->h_addr);
}

void connect_pool::buf_free() {
    if (buf) {
        for (int i = 0; i < max_connects; ++ i) {
            if (buf[i]) {
                free(buf[i]);
                buf[i] = NULL;
            }
        }
        free(buf);
        buf = NULL;
    }
}

void connect_pool::set_connections (int connects) {
    buf_free();
    max_connects = connects;
    
    buf = (char **) malloc(sizeof(char *) * max_connects);
    for (int i = 0; i < max_connects; ++ i)
        buf[i] = (char *) malloc(1500);
    
    create_udp(connects);
}

connect_pool::connect_pool(const char ip[], short port, int connects):sockets(NULL), max_connects(0), buf(NULL){
    set_addr (ip, port);
    set_connections (connects);
}
