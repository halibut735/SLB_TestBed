//
//  main.cpp
//  mesa_sender
//
//  Created by 中央政治局常委 on 16/8/8.
//  Copyright © 2016年 中央政治局常委. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include "mission.hpp"
#include "stream.hpp"
using namespace std;

int main(int argc, char *const* argv) {
    Mission test(1);
    test.parse_args(argc, argv);
    test.run();
    return 0;
}
