#include "./route_handler.h"

RouteHandler::RouteHandler(std::string path, http_parser::HttpMethod::Method method)
    : path_(std::move(path)), method_(method) {}

SyncRouteHandler::SyncRouteHandler(std::string path, http_parser::HttpMethod::Method method, RouteSyncCallback callback)
    : RouteHandler(path, method), callback_(callback){}

void SyncRouteHandler::Handle(http_parser::HttpRequest& request, http_parser::HttpResponse& response) const {
    if(callback_){
        callback_(request, response);
    }
}

AsyncRouteHandler::AsyncRouteHandler(std::string path, http_parser::HttpMethod::Method method, RouteAsyncCallback callback)
    : RouteHandler(path, method), callback_(callback){}

void AsyncRouteHandler::AssyncHandle(http_parser::HttpRequest& request, http_parser::HttpResponse& response, 
                                     boost::asio::io_service& io, std::function<void()> callback) const {
    
    if(callback_){
        callback_(request, response, io, callback);
    }
}