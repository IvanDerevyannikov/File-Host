#include "./route_handler.h"

RouteHandler::RouteHandler(std::string path, http_parser::HttpMethod::Method method)
    : path_(std::move(path)), method_(method) {}

SyncRouteHandler::SyncRouteHandler(std::string path, http_parser::HttpMethod::Method method, RouteSyncCallback callback)
    : RouteHandler(path, method), callback_(callback) {}

void SyncRouteHandler::Handle(http_parser::HttpRequest& request, http_parser::HttpResponse& response) const {
    if(callback_){
        callback_(request, response);
    }
}

AsyncRouteHandler::AsyncRouteHandler(std::string path, http_parser::HttpMethod::Method method, RouteAsyncCallback callback)
    : RouteHandler(path, method), callback_(callback) {}

void AsyncRouteHandler::AssyncHandle(boost::asio::io_context& io, 
                                     http_parser::HttpRequest& request, 
                                     http_parser::HttpResponse& response, 
                                     std::function<void(const boost::system::error_code& code)> callback) const {
    if(callback_){
        // callback_(io, request, response, callback);
    }
}