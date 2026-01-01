#include "error_handler_manager.h"
#include "../../http_parser/http_request_parser.h"

ErrorHandlerManager::errHandlPtr ErrorHandlerManager::manageErrorHandler(const ErrorHandlerFactory::ErrorHandlerType& errorType){

    return std::move(errorFactoryPtr_->createErrorHandler(errorType));
}

ErrorHandlerManager::errHandlPtr ErrorHandlerManager::manageErrorHandler(const std::string& acceptType){
    auto parsedAcceptType = http_parser::parseHeaderValue(acceptType,",");

    for(auto type: parsedAcceptType){
        if(type.find("image")!=std::string::npos){
            return manageErrorHandler(ErrorHandlerFactory::ErrorHandlerType::ImageErrorType);
        }
        else if(type.find("text")!=std::string::npos){
            return manageErrorHandler(ErrorHandlerFactory::ErrorHandlerType::HtmlErrorType);
        }
        else if(type.find("json")!=std::string::npos){
            return manageErrorHandler(ErrorHandlerFactory::ErrorHandlerType::JsonErrorType);
        }
    }
    
    return manageErrorHandler(ErrorHandlerFactory::ErrorHandlerType::DefaultType);
}

ErrorHandlerManager::errHandlPtr ErrorHandlerManager::manageErrorHandler(const http_parser::HttpRequest& req){
    auto parsedAcceptType = http_parser::parseHeaderValue(req.headers.at("Accept"), ",");
    for(auto type: parsedAcceptType){
        if(type.find("image")!=std::string::npos){
            return manageErrorHandler(ErrorHandlerFactory::ErrorHandlerType::ImageErrorType);
        }
        else if(type.find("text")!=std::string::npos){
            return manageErrorHandler(ErrorHandlerFactory::ErrorHandlerType::HtmlErrorType);
        }
        else if(type.find("json")!=std::string::npos){
            return manageErrorHandler(ErrorHandlerFactory::ErrorHandlerType::JsonErrorType);
        }
    }
    
    return manageErrorHandler(ErrorHandlerFactory::ErrorHandlerType::DefaultType);
}

 void ErrorHandlerManager::executeError(http_parser::HttpResponse& res, const http_parser::HttpRequest& req, http_parser::response::StatusCode statusCode){
    auto errorHandler = manageErrorHandler(req);
    routeError(res, std::move(errorHandler), statusCode);
 }

 void ErrorHandlerManager::executeError(http_parser::HttpResponse& res, const std::string& acceptType, http_parser::response::StatusCode statusCode){
    auto errorHandler = manageErrorHandler(acceptType);
    routeError(res, std::move(errorHandler), statusCode);
 }

 void ErrorHandlerManager::executeError(http_parser::HttpResponse& res, const ErrorHandlerFactory::ErrorHandlerType& errorType, http_parser::response::StatusCode statusCode){
    auto errorHandler = manageErrorHandler(errorType);
    routeError(res, std::move(errorHandler), statusCode);
 }

 void ErrorHandlerManager::routeError(http_parser::HttpResponse& res, errHandlPtr errorHandler, http_parser::response::StatusCode statusCode){
    switch (statusCode){
        case http_parser::response::StatusCode::bad_request:
            errorHandler->handle400Error(res);
            break;
        case http_parser::response::StatusCode::not_found:
            errorHandler->handle404Error(res);
            break;
        case http_parser::response::StatusCode::method_not_allowed:
            errorHandler->handle405Error(res);
            break;
        case http_parser::response::StatusCode::bad_gateway:
            errorHandler->handle500Error(res);
            break;
        default:
            errorHandler->handle404Error(res);
            break;
    }
 }