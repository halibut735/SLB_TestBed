//
//  mission.hpp
//  mesa_sender
//
//  Created by 中央政治局常委 on 16/8/8.
//  Copyright © 2016年 中央政治局常委. All rights reserved.
//

#ifndef mission_hpp
#define mission_hpp

#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <list>
#include "thread_pool.h"
#include "connect_pool.hpp"
#include "stream.hpp"
using namespace std;


class Mission {
public:
    Mission(int concurrency = 4);
    virtual ~Mission();
    bool parse_args(int argc, char *const* argv);
    void run();
    void send_packet(int tid, stream_base &pkts);
    
    
private:
    vector<stream_mission>      cycle_create;
    list<stream_mission>        missions;
    thread_pool                 tpool;
    connect_pool                cpoll;
    
    void crontab();
    void crontab_send();
    void crontab_create();
    void server_info(string & str);
};

#endif /* mission_hpp */
