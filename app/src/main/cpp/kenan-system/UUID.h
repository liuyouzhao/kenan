//
// Created by hujia on 7/18/20.
//

#ifndef KENAN_UUID_H
#define KENAN_UUID_H

#warning "UUID====================="

#include <string>
#include <sys/time.h>
#include <unistd.h>

namespace kenan_system {

std::string UUID_generate();
unsigned long UUID_gen_ulong();

}

#endif //KENAN_UUID_H
