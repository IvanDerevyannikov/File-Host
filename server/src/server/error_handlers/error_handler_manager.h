#ifndef ERROR_HANDLER_MANAGER_HEAD
#define ERROR_HANDLER_MANAGER_HEAD

#include "error_handler_factory.h"
#include "error_handler.h"
#include "../../http_parser/http_request.h"
#include "../../http_parser/http_response.h"
#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>


class ErrorHandlerManager{
public:
    using errFactPtr = boost::shared_ptr<ErrorHandlerFactory>;
    using errHandlPtr = std::unique_ptr<IErrorHandler>;

    ErrorHandlerManager(errFactPtr errorFactoryPtr): errorFactoryPtr_(errorFactoryPtr){
        
    }

    errHandlPtr manageErrorHandler(const http_parser::HttpRequest& req);
    errHandlPtr manageErrorHandler(const std::string& acceptType);
    errHandlPtr manageErrorHandler(const ErrorHandlerFactory::ErrorHandlerType& errorType);

    void executeError(http_parser::HttpResponse& res, const http_parser::HttpRequest& req, http_parser::response::StatusCode statusCode);
    void executeError(http_parser::HttpResponse& res, const std::string& acceptType, http_parser::response::StatusCode statusCode);
    void executeError(http_parser::HttpResponse& res, const ErrorHandlerFactory::ErrorHandlerType& errorType, http_parser::response::StatusCode statusCode);

    void asyncExecuteError();

private:
    void routeError(http_parser::HttpResponse& res, errHandlPtr errorHandler, http_parser::response::StatusCode statusCode);
    errFactPtr errorFactoryPtr_;
};

#endif