#ifndef STATIC_ROUTES_H
#define STATIC_ROUTES_H

#include "../handlers/static_file_handler.h"
#include "../server/multiplexer.h"


void asyncHandler(boost::asio::io_context& io, 
                  http_parser::HttpRequest& req, 
                  http_parser::HttpResponse& res, 
                  std::function<void(const boost::system::error_code& code)> callback) {
    auto& config = ConfigProvider::getConfig();
    // auto directoryPath = config.getValue<std::string>(configPath);
    auto fileName = http_parser::splitPath(req.startLine.url.getPath()).back();
        auto file = boost::asio::stream_file(
        io, 
        "/home/ivan/Programs/boost_asio_client_server_example/server/big_file.txt",
        boost::asio::file_base::read_only
    );
    boost::system::error_code ec;
    auto fileSize = file.size(ec);
    if(ec){
        callback(ec);
    }
    auto buffer = boost::make_shared<std::vector<char>>(fileSize);
    read_async_file_simple(file, buffer, [buffer, &res, &callback](const boost::system::error_code& code, size_t bytes_read) {
        res.setStartLine({"HTTP/1.1", http_parser::response::StatusCode::ok});
        res.setHeader({"Content-Type", "text; charset=UTF-8"});
        res.setHeader({"Content-Length", std::to_string(buffer->size())});
        res.setHeader({"Server", "FileHostServer/1.0"});
        res.setbody(buffer->data());
        callback(code);
    });

}

inline void registerStaticRoutes(Multiplexer& mult) {
    mult.AddRoute("/", http_parser::HttpMethod::GET, StaticFileHandlerFabrics::htmlHandler());
    mult.AddRoute("/css/{fileName}", http_parser::HttpMethod::GET, StaticFileHandlerFabrics::cssHandler());
    mult.AddRoute("/js/**/{fileName}", http_parser::HttpMethod::GET, StaticFileHandlerFabrics::jsHandler());
    mult.AddRoute("/image/svg/{fileName}", http_parser::HttpMethod::GET, StaticFileHandlerFabrics::svgHandler());
    mult.AddRoute("/image/jpeg/{fileName}", http_parser::HttpMethod::GET, StaticFileHandlerFabrics::jpegHandler());
    mult.AddRoute("/components/{fileName}", http_parser::HttpMethod::GET, StaticFileHandlerFabrics::htmlTemplateHandler());
    mult.AddRoute("/locales/{fileName}", http_parser::HttpMethod::GET, StaticFileHandlerFabrics::localesHandler());
}

// inline void registerAsyncRoutes(Multiplexer& mult, boost::asio::io_context& io){
//     mult.AddRoute("/test/test/test", 
//         http_parser::HttpMethod::GET, 
//         boost::make_shared<AsyncRouteHandler>(
//             "/test/test/test", 
//             http_parser::HttpMethod::GET,
//             [](boost::asio::io_context& io, http_parser::HttpRequest& req, http_parser::HttpResponse& res, std::function<void(const boost::system::error_code& code)> callback){
//                 asyncHandler(io, req, res, callback);
//             }
//         )
//     );
// }

#endif