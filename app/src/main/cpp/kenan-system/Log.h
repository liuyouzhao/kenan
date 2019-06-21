//
// Created by hujia on 22/06/19.
//

#ifndef KENAN_LOG_H
#define KENAN_LOG_H

#include <string>

namespace kenan_system
{

class Log
{
public:
    static void info(const char *tag, const char *format,...);
    static void debug(const char *tag, const char *format,...);
    static void error(const char *tag, const char *format,...);
};

}

#endif //KENAN_LOG_H
