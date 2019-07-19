//
// Created by hujia on 18/07/19.
//

#ifndef KENAN_HTTPCLIENT_H
#define KENAN_HTTPCLIENT_H

#include <string>
#include <map>

namespace kenan_system
{

class HttpClient
{
public:
    void* createGetRequest(std::string url, int port, std::string params[][2]);
    void doRequest(void *request);
    void setCallback(void* (cb)(char*, unsigned int, void* key));
    static std::map<void*, HttpClient*> requestMap;
private:
    void* (*onHttpCallback) (char *buffer, unsigned int length, void* key);


};

}


#endif //KENAN_HTTPCLIENT_H
