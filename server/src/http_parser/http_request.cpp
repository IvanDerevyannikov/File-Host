#include "http_request.h"


namespace http_parser{

    HttpMethod::HttpMethod(const std::string& methodStr): methodStr_(methodStr){
        if(methodStr == "GET") {method_ = HttpMethod::GET;}
        else if(methodStr == "HEAD") {method_ = HttpMethod::HEAD;}
        else if(methodStr == "POST")   {method_ = HttpMethod::POST;}
        else if(methodStr == "PUT")   {method_ = HttpMethod::PUT;}
        else if(methodStr == "DELETE") {method_ = HttpMethod::DELETE;}
        else if(methodStr == "CONNECT") {method_ = HttpMethod::CONNECT;}
        else if(methodStr == "OPTIONS") {method_ = HttpMethod::OPTIONS;}
        else if(methodStr == "TRACE") {method_ = HttpMethod::TRACE;}
        else if(methodStr == "PATCH") {method_ = HttpMethod::PATCH;}
        else{throw std::invalid_argument("Request Method is not valid " + methodStr);}
    }

    HttpMethod::HttpMethod(const HttpMethod::Method& method): method_(method){
        if(method_ == HttpMethod::GET) {methodStr_ = "GET";}
        else if(method_ == HttpMethod::HEAD) {methodStr_ = "HEAD";}
        else if(method_ == HttpMethod::POST) {methodStr_ = "POST";}
        else if(method_ == HttpMethod::PUT) {methodStr_ = "PUT";}
        else if(method_ == HttpMethod::DELETE) {methodStr_ = "DELETE";}
        else if(method_ == HttpMethod::CONNECT) {methodStr_ = "CONNECT";}
        else if(method_ == HttpMethod::OPTIONS) {methodStr_ = "OPTIONS";}
        else if(method_ == HttpMethod::TRACE) {methodStr_ = "TRACE";}
        else if(method_ == HttpMethod::PATCH) {methodStr_ = "PATCH";}
    }

    HttpMethod::Method HttpMethod::getMethod(){
        return method_;
    }

    std::string HttpMethod::toString(){
        return methodStr_;
    }

}