#ifndef ROUTE_HANDLER_HEAD
#define ROUTE_HANDLER_HEAD

#include <functional>
#include <string>
#include <boost/asio.hpp>
#include "../http_parser/http_request.h"
#include "../http_parser/http_response.h"

using RouteSyncCallback = std::function<void(http_parser::HttpRequest& request, http_parser::HttpResponse& response)>;
using RouteAsyncCallback = std::function<void(http_parser::HttpRequest& request, http_parser::HttpResponse& response, 
                    boost::asio::io_context& io, std::function<void()>)>;

class RouteHandler {
 public:

  RouteHandler(std::string path, http_parser::HttpMethod::Method method);

  virtual ~RouteHandler() = default;

  virtual void Handle(http_parser::HttpRequest& request, http_parser::HttpResponse& response) const {}
  virtual void AssyncHandle(http_parser::HttpRequest& request, http_parser::HttpResponse& response, 
                            boost::asio::io_context& io,std::function<void()>) const {}

  std::string GetPath() {return path_;}
  http_parser::HttpMethod::Method GetMethod() {return method_;}

 private:
  std::string path_;
  http_parser::HttpMethod::Method method_;
};

class SyncRouteHandler: public RouteHandler{
  public:
  
  SyncRouteHandler(std::string path, http_parser::HttpMethod::Method method, RouteSyncCallback callback);

  void Handle(http_parser::HttpRequest& request, http_parser::HttpResponse& response) const override;
  
  private:
  RouteSyncCallback callback_;
};

class AsyncRouteHandler: public RouteHandler{
  public:

  AsyncRouteHandler(std::string path, http_parser::HttpMethod::Method method, RouteAsyncCallback callback);

  void AssyncHandle(http_parser::HttpRequest& request, http_parser::HttpResponse& response, 
                    boost::asio::io_context& io, std::function<void()>) const override;

  private:
  RouteAsyncCallback callback_;
};

#endif  // ROUTE_HANDLER_H_