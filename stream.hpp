//
//  steam.hpp
//  mesa_sender
//
//  Created by 中央政治局常委 on 16/8/8.
//  Copyright © 2016年 中央政治局常委. All rights reserved.
//

#ifndef stream_hpp
#define stream_hpp

#include <stdio.h>
class Mission;
//将UDP的数据控制在1472字节

struct stream_base {
    unsigned int id;
    unsigned int offset;
    unsigned int len;
    unsigned int magic; // 0xAA start; 0xBB end; 0xEE error
    
    const static unsigned int START = 0x01;
    const static unsigned int END = 0x02;
    
    stream_base(unsigned int _id, unsigned int _offset = 0x10, unsigned int _len = 0, unsigned int _magic = 0xEE):
    id(_id),
    offset(_offset),
    len(_len),
    magic(_magic)
    {}
};

struct stream_pkts : stream_base {
    Mission * base;
    stream_pkts(Mission *a, unsigned int _id = 0, unsigned int _offset = 0, unsigned int _len = 0, unsigned int _magic = 0):
    stream_base(_id, _offset, _len, _magic),
    base(a)
    {}
};

struct stream_mission : stream_base {
    unsigned int speed;
    stream_mission(unsigned int _id = 0, unsigned int _offset = 0, unsigned int _len = 0, unsigned int _magic = 0, unsigned int _speed = 0):
    stream_base(_id, _offset, _len, _magic),
    speed(_speed)
    {}
};




#endif /* stream_hpp */
