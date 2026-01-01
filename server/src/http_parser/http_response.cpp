#include "http_response.h"

namespace http_parser{

    HttpResponseStart::HttpResponseStart(std::string version, response::StatusCode statusCode )
                                        : version_(version), statusCode_(statusCode){

    }

    std::string HttpResponseStart::getStartString(){
        return version_ + " " + std::to_string(statusCode_) + " " + statusCode.at(statusCode_);
    }

    HttpResponse::HttpResponse(HttpResponseStart startLine, std::vector<Header> headers, std::string body)
                               : startLine_(startLine), headers_(headers), body_(body){

    }

    std::string HttpResponse::toString(){
        std::string response = startLine_.getStartString() + "\r\n";
        for(auto& header: headers_){
            response += header.name + ": " + header.value + "\r\n";
        }
        response+="\r\n"+body_;
        return response;
    }

    void HttpResponse::setStartLine(const HttpResponseStart& startLine){
        this->startLine_ = startLine;
    }

    void HttpResponse::setStartLine(const std::string& version, const response::StatusCode& status){
        this->startLine_ = HttpResponseStart(version, status);
    }

    void HttpResponse::setHeader(const Header& header){
        this->headers_.push_back(header);
    }

    void HttpResponse::setbody(const std::string& body){
        this->body_= body;
    }
}