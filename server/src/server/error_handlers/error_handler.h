#ifndef ERROR_HANDLER_HEAD
#define ERROR_HANDLER_HEAD

#include "../../http_parser/http_response.h"
#include <memory>
#include "../request_error_exception.h"

class IErrorHandler{
public:

    virtual ~IErrorHandler() = default;

    virtual void handle400Error(http_parser::HttpResponse& response) = 0;
    virtual void handle404Error(http_parser::HttpResponse& response) = 0;
    virtual void handle405Error(http_parser::HttpResponse& response) = 0;
    virtual void handle500Error(http_parser::HttpResponse& response) = 0;
};


class DefaultErrorHandler: public IErrorHandler{
public:

    void handle400Error(http_parser::HttpResponse& response) override{}
    void handle404Error(http_parser::HttpResponse& response) override{}
    void handle405Error(http_parser::HttpResponse& response) override{}
    void handle500Error(http_parser::HttpResponse& response) override{}


};

class JsonErrorHandler: public IErrorHandler{
public:

    void handle400Error(http_parser::HttpResponse& response) override{}
    void handle404Error(http_parser::HttpResponse& response) override{}
    void handle405Error(http_parser::HttpResponse& response) override{}
    void handle500Error(http_parser::HttpResponse& response) override{}
};

class HtmlErrorHandler: public IErrorHandler{
public:

    void handle400Error(http_parser::HttpResponse& response) override;
    void handle404Error(http_parser::HttpResponse& response) override;
    void handle405Error(http_parser::HttpResponse& response) override;
    void handle500Error(http_parser::HttpResponse& response) override;
};


class ImageErrorHandler: public IErrorHandler{
public:

    void handle400Error(http_parser::HttpResponse& response) override;
    void handle404Error(http_parser::HttpResponse& response) override;
    void handle405Error(http_parser::HttpResponse& response) override;
    void handle500Error(http_parser::HttpResponse& response) override;

};


// class ErrorHandler: public IErrorHandler{
// public:

//     void handle400Error(http_parser::HttpResponse& response) override;
//     void handle404Error(http_parser::HttpResponse& response) override;
//     void handle405Error(http_parser::HttpResponse& response) override;
//     void handle500Error(http_parser::HttpResponse& response) override;

// private:

//     const std::unique_ptr<HtmlErrorHandler> htmlErrHandl_;
//     const std::unique_ptr<JsonErrorHandler> jsonErrHandl_;
//     const std::unique_ptr<ImageErrorHandler> imageErrHandl;
// };



#endif // ERROR_HANDLER_HEAD
