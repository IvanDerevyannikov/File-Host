#include "error_handler.h"
#include "../../config/config_provider.h"
#include <sstream>
#include <boost/filesystem.hpp>
#include <fstream>
#include <boost/json.hpp>


void HtmlErrorHandler::handle400Error(http_parser::HttpResponse& response){
    response.setStartLine("HTTP/1.1", http_parser::response::StatusCode::bad_request);
    
    auto errorFilePath = ConfigProvider::getConfig().getValue<std::string>({"directory","static"});
    std::ifstream errorFile = std::ifstream(errorFilePath+"/error400.html");
    if( !errorFile.is_open() ) {
        response.setHeader({"Content-Type", "text/plain"});
        response.setbody("400 Not Found");
    }
    else {
        std::stringstream stringBuffer;
        stringBuffer << errorFile.rdbuf();
        response.setHeader({"Content-Type", "text/html, charset=UTF-8"});
        response.setbody(stringBuffer.str());
    }
}


void HtmlErrorHandler::handle404Error(http_parser::HttpResponse& response){
    response.setStartLine(http_parser::HttpResponseStart("HTTP/1.1", http_parser::response::StatusCode::not_found));

    auto errorFilePath = ConfigProvider::getConfig().getValue<std::string>({"directory","static"});
    std::ifstream errorFile = std::ifstream(errorFilePath+"/error.html");
    if( !errorFile.is_open() ) {
        response.setHeader({"Content-Type", "text/plain"});
        response.setbody("404 Not Found");
    }
    else {
        std::stringstream stringBuffer;
        stringBuffer << errorFile.rdbuf();
        response.setHeader({"Content-Type", "text/html, charset=UTF-8"});
        response.setbody(stringBuffer.str());
    }
}

void HtmlErrorHandler::handle405Error(http_parser::HttpResponse& response){
    response.setStartLine("HTTP/1.1", http_parser::response::StatusCode::bad_request);
    
    auto errorFilePath = ConfigProvider::getConfig().getValue<std::string>({"directory","static"});
    std::ifstream errorFile = std::ifstream(errorFilePath+"/error400.html");
    if( !errorFile.is_open() ) {
        response.setHeader({"Content-Type", "text/plain"});
        response.setbody("405 Not Found");
    }
    else {
        std::stringstream stringBuffer;
        stringBuffer << errorFile.rdbuf();
        response.setHeader({"Content-Type", "text/html, charset=UTF-8"});
        response.setbody(stringBuffer.str());
    }
}

void HtmlErrorHandler::handle500Error(http_parser::HttpResponse& response){
    response.setStartLine(http_parser::HttpResponseStart("HTTP/1.1",http_parser::response::StatusCode::internal_server_error));

    auto errorFilePath = ConfigProvider::getConfig().getValue<std::string>({"directory","static"});
    std::ifstream errorFile = std::ifstream(errorFilePath+"/error.html");
    if( !errorFile.is_open() ) {
        response.setHeader({"Content-Type", "text/plain"});
        response.setbody("500 Not Found");
    }
    else {
        std::stringstream stringBuffer;
        stringBuffer << errorFile.rdbuf();
        response.setHeader({"Content-Type", "text/html, charset=UTF-8"});
        response.setbody(stringBuffer.str());
    }
}

void ImageErrorHandler::handle400Error(http_parser::HttpResponse& response){
        response.setStartLine("HTTP/1.1", http_parser::response::not_found);
    boost::json::value errorJson = {
        {
            "error", {
                { "code", 404 },
                { "message", "Image not found"}
            }
        }
    };
    response.setHeader({"Content-Type", "aplication/json; charset=utf-8"});
    response.setbody(boost::json::serialize(errorJson));
}

void ImageErrorHandler::handle404Error(http_parser::HttpResponse& response){
    response.setStartLine("HTTP/1.1", http_parser::response::not_found);
    boost::json::value errorJson = {
        {
            "error", {
                { "code", 404 },
                { "message", "Image not found"}
            }
        }
    };
    response.setHeader({"Content-Type", "aplication/json; charset=utf-8"});
    response.setbody(boost::json::serialize(errorJson));

}

void ImageErrorHandler::handle405Error(http_parser::HttpResponse& response){
    response.setStartLine("HTTP/1.1", http_parser::response::method_not_allowed);
    boost::json::value errorJson = {
        {
            "error", {
                { "code", 405 },
                { "message", "Method_not_allowed"}
            }
        }
    };
    response.setHeader({"Content-Type", "aplication/json; charset=utf-8"});
    response.setbody(boost::json::serialize(errorJson));
}

void ImageErrorHandler::handle500Error(http_parser::HttpResponse& response){
    response.setStartLine("HTTP/1.1", http_parser::response::internal_server_error);
    boost::json::value errorJson = {
        {
            "error", {
                { "code", 500 },
                { "message", "Internal server error"}
            }
        }
    };
    response.setHeader({"Content-Type", "aplication/json; charset=utf-8"});
    response.setbody(boost::json::serialize(errorJson));
}