#include "./file_handler.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/json.hpp>

#include "../../config/config_provider.h"
#include "../../dto/file_dto.h"
#include "../../dto/error_dto.h"

FileHandler::FileHandler()
    : fileService_(StoragePaths(
          ConfigProvider::getConfig().getValue<std::string>({"storage", "root"}))) {}

//TODO make response in specific function
Multiplexer::RoutePtr FileHandler::getFiles() {
    return boost::make_shared<SyncRouteHandler>(
        "/api/v1/files",
        http_parser::HttpMethod::Method::GET,
        [this](http_parser::HttpRequest& request, http_parser::HttpResponse& response) {
            auto res = fileService_.list();
            std::visit([&response](auto&& value){
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, ErrorResponse>) {
                    const std::string json = boost::json::serialize(errorToJson(value));
                    response.setStartLine({"HTTP/1.1", http_parser::response::StatusCode::internal_server_error});
                    response.setHeader({"Content-Type", "application/json"});
                    response.setHeader({"Content-Length", std::to_string(json.size())});
                    response.setHeader({"Server", "FileHostServer/1.0"});
                    response.setbody(json); 
                }
                else {
                    const std::string json = boost::json::serialize(listResponse(value));
                    response.setStartLine({"HTTP/1.1", http_parser::response::StatusCode::ok});
                    response.setHeader({"Content-Type", "application/json"});
                    response.setHeader({"Content-Length", std::to_string(json.size())});
                    response.setHeader({"Server", "FileHostServer/1.0"});
                    response.setbody(json); 
                }
            }, res);
        }
    );
}