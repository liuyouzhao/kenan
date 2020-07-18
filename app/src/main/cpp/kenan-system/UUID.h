//
// Created by hujia on 7/18/20.
//

#ifndef KENAN_UUID_H
#define KENAN_UUID_H

#include <string>
#include <sys/time.h>
#include <unistd.h>

namespace kenan_system {

std::string UUID_generate() {
    struct timeval start_time, end_time;
    long milli_time, seconds, useconds;
    gettimeofday(&start_time, NULL);

    char uuid[128] = {0};
    sprintf(uuid, "%d", end_time.tv_usec);
    int i = 0;
    for(; i < 128; i ++) {
        if(uuid[i] == 0) {
            uuid[i] = rand() % 42 + 48;
        }
    }
    return std::string(uuid);
}

unsigned long UUID_gen_ulong() {
    struct timeval start_time, end_time;
    long milli_time, seconds, useconds;
    gettimeofday(&start_time, NULL);
    return end_time.tv_usec;
}

}

#endif //KENAN_UUID_H
