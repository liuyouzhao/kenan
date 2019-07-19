//
// Created by hujia on 18/07/19.
//

#include "HttpClient.h"
#include "libnhr.h"

namespace kenan_system
{

std::map<void*, HttpClient*> HttpClient::requestMap;

void* HttpClient::createGetRequest(std::string url, int port, std::string params[][2])
{
    // Define variable or field for the request
    nhr_request _request = NULL;

    // Create request object
    _request = nhr_request_create();

    // Combined url: "http://api.ipify.org"
    nhr_request_set_url(_request, "http", "api.ipify.org", "/", 80);

    nhr_request_set_method(_request, nhr_method_GET); // GET

    return _request;
}

static void on_request_error(nhr_request request, nhr_error_code error_code) {
    request = NULL; // Clean up previously stored request variable or field
    //TODO: process `error_code`
}

static void on_request_response(nhr_request request, nhr_response response) {
    request = NULL; // Clean up previously stored request variable or field
    char * body = (char*) nhr_response_get_body(response);
    unsigned int body_length = nhr_response_get_body_length(response);
    if (body && body_length) {
        if(HttpClient::requestMap[(void*)request] == NULL)
        {

        }
    }
}
void HttpClient::doRequest(void *request)
{
    // Set request callbacks
    nhr_request_set_on_recvd_response((nhr_request)request, &on_request_response);
    nhr_request_set_on_error((nhr_request)request, &on_request_error);

    nhr_request_send((nhr_request)request);
}

void HttpClient::setCallback(void* (cb)(char*, unsigned int, void* key))
{
    this->onHttpCallback = cb;
}

}