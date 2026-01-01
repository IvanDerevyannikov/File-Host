#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>
#include "http_url.h"

namespace http_parser{
    
    class HttpMethod{
        public:
        enum Method{
            GET = 1,
            HEAD = 2,
            POST = 3,
            PUT = 4,
            DELETE = 5,
            CONNECT = 6,
            OPTIONS = 7,
            TRACE = 8,
            PATCH = 9,
        };

        HttpMethod()=default;

        explicit HttpMethod(const std::string& methodStr);
        explicit HttpMethod(const HttpMethod::Method& method);

        Method getMethod();

        std::string toString();
        
        private:
        std::string methodStr_;
        HttpMethod::Method method_;
    };


    struct HttpStartString{

        HttpStartString& operator=(const HttpStartString& other){
            this->method = other.method;
            this->url = other.url;
            this->version = other.version;
            return *this;
        }

        HttpMethod method;
        http_parser::Url url;
        std::string version;
    };


    struct HttpRequest{

        HttpRequest& operator=(const HttpRequest& req){
            this->body= req.body;
            this->headers = req.headers;
            this->startLine = req.startLine;
            return *this;
        }
        HttpStartString startLine;
        std::unordered_map<std::string,std::string> headers;

        std::string body;
    };

}