#include "static_file_handler.h"
#include "../server/route_handler.h"
#include "../config/config.h"
#include "../config/config_provider.h"
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/iterator_range.hpp>



Multiplexer::RoutePtr StaticFileHandlerFabrics::create(
    const std::string& path,
    const std::vector<std::string>& configPath,
    http_parser::HttpMethod::Method method,
    const std::string& contentType
) {
    return boost::make_shared<SyncRouteHandler>(
        path, 
        method,
        [path, configPath, contentType](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto& config = ConfigProvider::getConfig();
            auto directoryPath = config.getValue<std::string>(configPath);
            auto fileName = http_parser::splitPath(request.startLine.url.getPath()).back();
            std::optional<FileData> fileData;
            if(path=="/"){
                fileData = readFile(directoryPath+"/index.html");
            } 
            else{
                fileData = readFile(directoryPath + "/" + fileName);
            }

            response.setStartLine({"HTTP/1.1", http_parser::response::StatusCode::ok});
            response.setHeader({"Content-Type", contentType});
            response.setHeader({"Content-Length", std::to_string(fileData->size)});
            response.setHeader({"Server", "FileHostServer/1.0"});
            //TO DO
            //response.setHeader({"Cache-Control", "public, max-age=3600 immutable"});
            response.setbody(fileData->data);
        });
}

Multiplexer::RoutePtr StaticFileHandlerFabrics::createWildcardPathHandler(
    const std::string& path,
    const std::vector<std::string>& configPath,
    http_parser::HttpMethod::Method method,
    const std::string& contentType
){
    return boost::make_shared<SyncRouteHandler>(
        path, 
        method,
        [path, configPath, contentType](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto splitPaths = http_parser::splitPath(request.startLine.url.getPath());
            auto& config = ConfigProvider::getConfig();
            auto staticPath = config.getValue<std::string>(configPath);

            auto wildcardPos = path.find("/**/");
            auto pathBeforeWildcard = path.substr(0, wildcardPos);

            if (!pathBeforeWildcard.empty() && pathBeforeWildcard.back() == '/') {
                pathBeforeWildcard.pop_back();
            }

            auto lastSlashPos = pathBeforeWildcard.find_last_of('/');
            auto folderString = pathBeforeWildcard.substr(lastSlashPos + 1);

            auto wildcardStartIterator = std::find(splitPaths.begin(), splitPaths.end(), folderString);
            auto rangeAfterStart = boost::make_iterator_range(wildcardStartIterator+1, splitPaths.end());
            auto pathStr = boost::algorithm::join(rangeAfterStart, "/");

            std::optional<FileData> fileData = readFile(staticPath + "/" + pathStr);


            response.setStartLine(http_parser::HttpResponseStart("HTTP/1.1", http_parser::response::StatusCode::ok));
            response.setHeader({"Content-Type", contentType});
            response.setHeader({"Content-Length", std::to_string(fileData->size)});
            response.setHeader({"Server", "FileHostServer/1.0"});
            //TO DO
            //response.setHeader({"Cache-Control", "public, max-age=3600 immutable"})
            response.setbody(fileData->data);
        }
    );
}

Multiplexer::RoutePtr StaticFileHandlerFabrics::htmlHandler(){
    return create(
        "/",
        {"directory", "static"}, 
        http_parser::HttpMethod::Method::GET,
        "text/html; charset=UTF-8"
    );
}

Multiplexer::RoutePtr StaticFileHandlerFabrics::jsHandler(){
    return createWildcardPathHandler(
        "/js/**/{fileName}",
        {"directory", "js"},
        http_parser::HttpMethod::GET,
        "application/javascript; charset=UTF-8"
    );
}

Multiplexer::RoutePtr StaticFileHandlerFabrics::cssHandler(){
    return create(
        "/css/{fileName}",
        {"directory", "css"},
        http_parser::HttpMethod::GET,
        "text/css; charset=UTF-8"
    );
}

Multiplexer::RoutePtr StaticFileHandlerFabrics::htmlTemplateHandler(){
    return create(
        "/components/{fileName}",
        {"directory", "html_templates"},
        http_parser::HttpMethod::GET,
        "text/html; charset=UTF-8"
    );
}

Multiplexer::RoutePtr StaticFileHandlerFabrics::svgHandler(){
    return create(
        "/image/svg/{fileName}",
        {"directory", "image", "svg"},
        http_parser::HttpMethod::GET,
        "image/svg+xml"
    );
}

Multiplexer::RoutePtr StaticFileHandlerFabrics::jpegHandler(){
    return create(
        "/image/jpeg/{fileName}",
        {"directory", "image", "jpeg"},
        http_parser::HttpMethod::GET,
        "image/jpeg"
    );
}

Multiplexer::RoutePtr StaticFileHandlerFabrics::localesHandler(){
    return create(
        "/locales/{fileName}",
        {"directory","locales"},
        http_parser::HttpMethod::GET,
        "text/json"
    );
}
