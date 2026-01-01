#ifndef REQUEST_ERROR_EXCEPTION_HEAD
#define REQUEST_ERROR_EXCEPTION_HEAD

#include <exception>
#include <string>
#include "../http_parser/http_response.h"

class RequestErrorExtension: public std::exception{

public:

    RequestErrorExtension(const std::string& message, const http_parser::response::StatusCode& statusCode):
        message_(message), statusCode_(statusCode){
    }

    const char * what() const noexcept override{
        return message_.c_str();
    }
    http_parser::response::StatusCode statusCode() const noexcept{
        return statusCode_;
    }

private:
    http_parser::response::StatusCode statusCode_;
    std::string message_;
};


#endif