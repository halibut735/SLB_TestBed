//
//  mission.cpp
//  mesa_sender
//
//  Created by 中央政治局常委 on 16/8/8.
//  Copyright © 2016年 中央政治局常委. All rights reserved.
//

#include "mission.hpp"
#include <iostream>
#include <assert.h>
#include <string>
using namespace std;



bool Mission::parse_args(int argc, char *const* argv) {
    int oc = 0;
    char ec = 0;
    
    int concurrency = 0;
    unsigned int id = 0, speed = 0, len = 0;
    string b_opt_arg;
    while((oc = getopt(argc, argv, "d:i:p:m:b:s:h:")) != -1)
    {
        switch(oc)
        {
            case 'd':
                //destination
                b_opt_arg = optarg;
                server_info(b_opt_arg);
                break;
                
            case 'i':
                //the only one stream_id
                assert(!concurrency);
                concurrency = 1;
                b_opt_arg = optarg;
                id = stoi(b_opt_arg);
                cout << "i: " << b_opt_arg << endl;
                break;
                
            case 'p':
                //concurrency
                assert(!concurrency);
                b_opt_arg = optarg;
                cout << "p: " << b_opt_arg << endl;
                break;
                
            case 'm':
                //new stream every second
                assert(!concurrency);
                b_opt_arg = optarg;
                cout << "m: " << b_opt_arg << endl;
                break;
                
            case 'h':
                //read file
                b_opt_arg = optarg;
                cout << "h: " << b_opt_arg << endl;
                break;
                
            case 'b':
                //stream length
                b_opt_arg = optarg;
                len = stoi(b_opt_arg);
                cout << "b: " << b_opt_arg << endl;
                break;
                
            case 's':
                //stream speed
                b_opt_arg = optarg;
                speed = stoi(b_opt_arg);
                cout << "s: " << b_opt_arg << endl;
                break;
                
                
            case '?':
                ec = (char)optopt;
                printf("无效的选项字符 \' %c \'!\n", ec);
                break;
            case ':':
                printf("缺少选项参数！\n");
                break;
            default:
                printf("无关参数\n");
                break;
        }
    }
    if (concurrency == 1) {
        stream_mission one;
        one.id = id;
        one.len = len;
        one.speed = speed;
        one.magic = stream_base::START;
        one.offset = sizeof(stream_mission);
        missions.push_back(one);
    } else if (concurrency) {
        ;
    }
    return true;
}

void Mission::send_packet(int tid, stream_base &pkts) {
    cpoll.send(tid, pkts);
}


void Mission::crontab_send() {
    stream_pkts tmp(this);
    for (list<stream_mission>::iterator it = missions.begin(); it != missions.end(); ++ it) {
        tmp.id = it->id;
        tmp.magic = it->magic | (it->len <= it->speed ? stream_base::END : 0);
        tmp.offset = it->offset;
        tmp.len = it->speed <= it->len ? it->speed : it->len;
        tpool.put_mission(tmp);
        
        it->len -= tmp.len;
        it->magic = 0;
        if (!it->len)
            missions.erase(it);
    }
}


void Mission::crontab_create() {
    for (auto each : cycle_create) {
        missions.push_back(each);
    }
}

void Mission::crontab() {
    crontab_send();
    crontab_create();
}

void Mission::run() {
    struct timeval countdown;
    countdown.tv_sec = 1;
    countdown.tv_usec = 0;
    //这里必须有true死循环啊。。不然Mission对象销毁了，this指针指向哪里了，其它线程没法用了啊？
    while (true) {
        select(0, NULL, NULL, NULL, &countdown);
        cout << missions.size() << endl;
        crontab();
    }
}

Mission::Mission(int concurrency):
tpool(concurrency),
cpoll(concurrency),
cycle_create(),
missions()
{}


Mission::~Mission(){};

void Mission::server_info(string & str) {
    int pos = 0;
    short port = 0;
    string sstr;
    pos = (int) str.find(':');
    sstr = str.substr(0, pos);
    port = atoi(str.substr(pos + 1).c_str()) ;
#ifdef DEBUG
    cout << "d: " << str << endl;
    cout << sstr << endl;
    cout << port << endl;
#endif
    cpoll.set_addr(sstr.c_str(), port);
}






