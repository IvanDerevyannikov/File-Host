/**
 *  danilod100 at gmail.com
 *  Compile with: g++ async_http_server.cpp -o async_http_server -lboost_system -lboost_thread -lpthread
 *
 * */
//#define BOOST_ASIO_HAS_FILE 1
// #define BOOST_ASIO_HAS_IO_URING 1 // on linux
#include <iostream>
#include <fstream>
#include <sstream>
#include <ostream>
#include <vector>
#include <istream>
#include <ctime>
#include <string>
#include <thread>
#include <filesystem>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <yaml-cpp/yaml.h>

#include "./http_parser/http_response.h"
#include "./http_parser/http_request.h"
#include "./http_parser/http_request_parser.h"
#include "./server/multiplexer.h"
#include "./server/route_handler.h"
#include "./server/server.h"
#include "./config/config.h"
#include "./config/config_provider.h"

using boost::asio::ip::tcp;

class HttpServer; // forward declaration



void handleRead(boost::system::error_code ec, size_t n) {
    std::cout << ec.message() << ": " << n << " bytes" << std::endl;
}

std::string getGmtTime(){
    boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
    auto pFacet = new boost::posix_time::time_facet("%a, %d %b %Y %H:%M:%S GMT");

    std::ostringstream oss;
    oss.imbue(std::locale(std::locale::classic(),pFacet));
    oss << now;
    return oss.str();
}


// class Request : public boost::enable_shared_from_this<Request>
// {
//     static std::string make_daytime_string()
//     {
//           std::time_t now = std::time(0);
//           return std::ctime(&now);
//     }

//     // member variables
//     HttpServer& server;
//     boost::asio::io_context& io;
//     boost::asio::streambuf request;

//     void handleRead(boost::system::error_code ec, size_t n) {
//         std::cout << ec.message() << ": " << n << " bytes" << std::endl;
//     }

//     void afterRead( const boost::system::error_code& ec, std::size_t bytes_transferred)
//     {
//         std::istream request_stream(&request);
//         // std::string request_line;
//         // std::getline(request_stream, request_line);

//         std::string file_name = "../src/static/index.html";
//         std::cout << "Текущая рабочая директория: " 
//               << std::filesystem::current_path() << '\n';
//         std::ifstream file(file_name);
//         if (!file) {
//             std::cerr << "Ошибка открытия файла\n";
//         }
//         // std::ostringstream buffer;
//         // buffer << file.rdbuf(); // Считывание всего содержимого файла
//         // std::string content = buffer.str();

//         // auto fileStream = std::make_shared<boost::asio::stream_file>(socket->get_executor(), file_name, boost::asio::stream_file::read_only);
//         // auto buffer = std::make_shared<std::vector<char>>(10000);
//         // //std::cout << fileStream.size() << '\n';
//         // boost::asio::async_read(*fileStream,boost::asio::buffer(*buffer),[this,fileStream,buffer](boost::system::error_code ec, size_t n){
//         //     std::cout << ec.message() << ": " << n << " bytes" << std::endl;
//         //     std::cout << fileStream->size();
//         //     std::ostringstream response_stream;
//         //     response_stream.write(buffer->data(), n);
//         //     std::string ser(response_stream.str());
//         //     std::cout << ser;
//         //     //this->afterRead(ec,2);
//         // });

//         // std::string request_data((std::istreambuf_iterator<char>(request_stream)),
//         //             std::istreambuf_iterator<char>());
//         // auto parsedRequest = http_parser::parseHttpRequest(request_data);

//         // // Парсим метод и путь из первой строки запроса
//         // // std::istringstream request_line_stream(request_line);
//         // std::string method, path, version;
//         // method = parsedRequest.startLine.method.toString();
//         // //path = parsedRequest.startLine.url;
//         // version = parsedRequest.startLine.version;
//         // // request_line_stream >> method >> path >> version;
//         // std::cout << "Request line2: " << method <<"    " << path << "      "<< version<< std::endl;
//         // if(path=="/"){
//         //     getMainPaige();
//         // }
//         // else if(path==""){
//         //     getMainPaige();
//         // }
//         // else if(path=="/image.jpeg?v=1"){
//         //     getImage1();
//         // }
//         // else if(path=="/image.jpeg?v=2"){
//         //     getImage2();
//         }
//         // done reading, writes answer (yes, we ignore the request);
//         // boost::asio::streambuf response;
//         // std::ostream res_stream(&response);

//         // // Gets daytime string (this function should return a valid HTML string with the image)
//         // std::string time = make_daytime_string();

//         // // Create HTML content with an image tag
//         // std::string html_content = "<html><head><title>Daytime</title></head><body>"
//         //                         "<h1>" + time + "</h1>"
//         //                         "<img src=\"../../image.jpeg\" alt=\"Image\">"
//         //                         "<img src=\"../image.jpeg\" alt=\"Image\">"
//         //                         "</body></html>";

//         // // Write HTTP response
//         // res_stream << "HTTP/1.0 200 OK\r\n"
//         //         << "Content-Type: text/html; charset=UTF-8\r\n"
//         //         << "Content-Length: " << html_content.length() << "\r\n\r\n"
//         //         << html_content;

//         // boost::asio::async_write(*socket, response, 
//         //             boost::bind(&Request::afterWrite, shared_from_this(),
//         //             boost::asio::placeholders::error, 2));
//     }
    
//     void afterWrite( const boost::system::error_code& ec, std::size_t bytes_transferred)
//     {
//         // done writing, closing connection
//         //socket->close();
//     }

//     void getMainPaige(){
//         boost::asio::streambuf response;
//         std::ostream res_stream(&response);

//         // Gets daytime string (this function should return a valid HTML string with the image)
//         std::string time = make_daytime_string();

//         // Create HTML content with an image tag
//         std::string html_content = "<html><head><title>Daytime</title></head><body>"
//                                 "<h1>" + time + "</h1>"
//                                 "<img src=\"../../image.jpeg?v=1\" alt=\"Image\">"
//                                 "<img src=\"../image.jpeg?v=2\" alt=\"Image\">"
//                                 "</body></html>";

//         // Write HTTP response
//         http_parser::HttpResponseStart responseStart("HTTP/1.0", http_parser::response::StatusCode::ok);
//         std::vector<http_parser::Header> headers;
//         headers.push_back({"Content-Type", "text/html; charset=UTF-8"});
//         headers.push_back({"Content-Length", std::to_string(html_content.length())});
//         http_parser::HttpResponse response1(responseStart, headers, html_content);
//         // res_stream << "HTTP/1.0 200 OK\r\n"
//         //         << "Content-Type: text/html; charset=UTF-8\r\n"
//         //         << "Content-Length: " << html_content.length() << "\r\n\r\n"
//         //         << html_content;
//         res_stream << response1.toString();
//         //std::cout << response1.toString();
//         boost::asio::async_write(*socket, response, 
//                     boost::bind(&Request::afterWrite, shared_from_this(),
//                     boost::asio::placeholders::error, 2));

//     }

//     void getImage1(){
//         boost::asio::streambuf response;
//         std::ostream res_stream(&response);
//         //boost::asio::stream_file file_stream(this->server.io_context);
//         //file_stream.open("../image.jpeg");
//         //file_stream.open("../image.jpeg", std::ios::binary);
//         std::ifstream image_file("../image.jpeg", std::ios::in | std::ios::binary);
//         std::ostringstream oss;
//         oss << image_file.rdbuf();
//         std::string image_data = oss.str();

//         res_stream << "HTTP/1.0 200 OK\r\n"
//                    << "Content_Type: image/jpeg\r\n"
//                    << "Content-Lenght: " << image_data.length() << "\r\n\r\n";
                           

//         res_stream.write(image_data.data(), image_data.size());

//         boost::asio::async_write(*socket, response, 
//                     boost::bind(&Request::afterWrite, shared_from_this(),
//                     boost::asio::placeholders::error, 2));
//     }

//     void getImage2(){
//         boost::asio::streambuf response;
//         std::ostream res_stream(&response);
//         std::ifstream image_file("../../image.jpeg", std::ios::in | std::ios::binary);
//         std::ostringstream oss;
//         oss << image_file.rdbuf();
//         std::string image_data = oss.str();

//         res_stream << "HTTP/1.0 200 OK\r\n"
//                    << "Content_Type: image/jpeg\r\n"
//                    << "Content-Lenght: " << image_data.length() << "\r\n\r\n";
                           

//         res_stream.write(image_data.data(), image_data.size());

//         boost::asio::async_write(*socket, response, 
//                     boost::bind(&Request::afterWrite, shared_from_this(),
//                     boost::asio::placeholders::error, 2));
//     }        

//     public:

//         boost::shared_ptr<tcp::socket> socket;
//         Request(HttpServer& server);
//         void answer()
//         {
//             if (!socket) return;
            
//             // reads request till the end
//             // boost::asio::async_read_until(*socket, request, "\r\n\r\n",
//             //                               boost::bind(&Request::afterRead, shared_from_this(), boost::asio::placeholders::error,2));
//         }
        
// };


class HttpServer
{
    public:
    
    HttpServer(boost::asio::io_context& io, unsigned int port) : io_context(io), acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {}
    ~HttpServer() { if (sThread) sThread->join(); }
    
    // void Run()
    // {
    //     sThread.reset(new boost::thread(boost::bind(&HttpServer::thread_main, this)));
    // }

    boost::asio::io_context& io_context;

    private:
    tcp::acceptor acceptor;
    boost::shared_ptr<boost::thread> sThread;

    // void thread_main()
    // {
    //     // adds some work to the io_context
    //     start_accept();
    //     io_context.run();
    // }

    // void start_accept()
    // {
    //     boost::shared_ptr<Request> req (new Request(*this));
    //     acceptor.async_accept(*req->socket,
    //                          [this, req](const boost::system::error_code& error){
    //                             if (!error) { req->answer(); }
    //                             this->start_accept();
    //                         });
    // }

    // void handle_accept(boost::shared_ptr<Request> req, const boost::system::error_code& error)
    // {
    //     if (!error) { req->answer(); }
    //     start_accept();
    // }
};

// Request::Request(HttpServer& server): server(server), io(server.io_context)
// {
//     socket.reset(new tcp::socket(server.io_context));
// }

struct Point {
    int x, y;

    //auto operator<=>(const Point&) const = default;  // Космический корабль
};
//using namespace httpParser;


int main (int argc, char *argv[])
{
   std::cout << "Boost version: " << BOOST_LIB_VERSION << std::endl;
   boost::asio::io_context io;
   //boost::asio::stream_file file(io);
   //HttpServer server(io, 8080);
   //server.Run();
   std::cout << getGmtTime() << '\n';
   //auto requestStr = parseHeader("first: second; sfesef;sfesdf; sfefse;    ");
   //httpParser.sum(1,2);
    
    Multiplexer mult;

    Multiplexer::RoutePtr handler = boost::make_shared<SyncRouteHandler>("/", http_parser::HttpMethod::Method::GET, 
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto& config = ConfigProvider::getConfig();
            auto staticPath = config.getValue<std::string>({"directory", "static"});
            auto file = std::ifstream(staticPath+"/index.html");
            std::ostringstream oss;
            oss << file.rdbuf();
            std::string htmp_data = oss.str();
            if(!file.is_open()){
                std::cout << "\n" << "not open" << '\n';
            }
            response.setStartLine(http_parser::HttpResponseStart("HTTP/1.1", http_parser::response::StatusCode::ok));
            response.setHeader({"Content-Type", "text/html; charset=UTF-8"});
            response.setHeader({"Content-Length", std::to_string(htmp_data.length())});
            response.setbody(htmp_data);
        }
    );
    auto async_handler = boost::make_shared<AsyncRouteHandler>(
        "/go/go",
        http_parser::HttpMethod::Method::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response, boost::asio::io_context& io,std::function<void()> completion) {
        // std::string request_line;
        // std::getline(request_stream, request_line);

        std::string file_name = "../src/static/index.html";
        std::cout << "Текущая рабочая директория: " 
              << std::filesystem::current_path() << '\n';
        std::ifstream file(file_name);
        if (!file) {
            std::cerr << "Ошибка открытия файла\n";
        }
        // std::ostringstream buffer;
        // buffer << file.rdbuf(); // Считывание всего содержимого файла
        // std::string content = buffer.str();

        // auto fileStream = std::make_shared<boost::asio::stream_file>(io.get_executor(), file_name, boost::asio::stream_file::read_only);
        // auto buffer = std::make_shared<std::vector<char>>(10000);
        completion();
        }
    );

    auto staticHandler = boost::make_shared<SyncRouteHandler>(
        "/test/{fileName}",
        http_parser::HttpMethod::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto splitPaths = http_parser::splitPath(request.startLine.url.getPath());
            std::cout<< "requst path file" << splitPaths.back() << '\n';
            response.setStartLine(http_parser::HttpResponseStart("HTTP/1.1", http_parser::response::StatusCode::ok));
            response.setHeader({"Content-Type", "text; charset=UTF-8"});
            response.setHeader({"Content-Length", std::to_string(splitPaths.back().length())});
            response.setbody(splitPaths.back());
        }
    );

    auto jsHandler = boost::make_shared<SyncRouteHandler>(
        "/js/{fileName}",
        http_parser::HttpMethod::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto splitPaths = http_parser::splitPath(request.startLine.url.getPath());
            std::cout<< "requst path file" << splitPaths.back() << '\n';
            auto& config = ConfigProvider::getConfig();
            auto staticPath = config.getValue<std::string>({"directory", "static"});

            auto file = std::ifstream(staticPath + "/js/" + splitPaths.back());
            std::ostringstream oss;
            oss << file.rdbuf();
            std::string js_data = oss.str();

            response.setStartLine(http_parser::HttpResponseStart("HTTP/1.1", http_parser::response::StatusCode::ok));
            response.setHeader({"Content-Type", "application/javascript; charset=UTF-8"});
            response.setHeader({"Content-Length", std::to_string(js_data.length())});
            response.setbody(js_data);
        }
    );

    auto htmlTemplateHandler = boost::make_shared<SyncRouteHandler>(
        "/components/{fileName}",
        http_parser::HttpMethod::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto& config = ConfigProvider::getConfig();
            auto htmlTemplatesPath = config.getValue<std::string>({"directory", "html_templates"});
            auto splitPath = http_parser::splitPath(request.startLine.url.getPath());
            auto htmlTemplateFile = std::ifstream(htmlTemplatesPath + "/" +splitPath.back());
            std::ostringstream htmlTemplateStream;
            htmlTemplateStream << htmlTemplateFile.rdbuf();
            auto htmlTemplateString = htmlTemplateStream.str();

            response.setStartLine(http_parser::HttpResponseStart("HTTP/1.1", http_parser::response::StatusCode::ok));

            response.setHeader({"Content-Type", "text; charset=UTF-8"});
            response.setHeader({"Content-Lenght", std::to_string(htmlTemplateString.length())});

            response.setbody(htmlTemplateString);
        }
    );

    auto imageSvgHandler = boost::make_shared<SyncRouteHandler>(
        "/image/svg/{fileName}",
        http_parser::HttpMethod::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto& config = ConfigProvider::getConfig();
            auto splitUrl = http_parser::splitPath(request.startLine.url.getPath());
            auto imagePath = config.getValue<std::string>({"directory", "image"}) + "/svg";
            auto imageFile = std::ifstream(imagePath + "/" + splitUrl.back());
            std::ostringstream imageStream;
            imageStream << imageFile.rdbuf();
            auto imageString = imageStream.str();

            response.setStartLine({"HTTP/1.1", http_parser::response::StatusCode::ok});

            response.setHeader({"Content-Type", "image/svg+xml"});
            response.setHeader({"Content-Length", std::to_string(imageString.length())});

            response.setbody(imageString);
        }
    );

    auto imageJpegHandler = boost::make_shared<SyncRouteHandler>(
        "/image/jpeg/{fileName}",
        http_parser::HttpMethod::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto& config = ConfigProvider::getConfig();
            auto splitUrl = http_parser::splitPath(request.startLine.url.getPath());
            auto imagePath = config.getValue<std::string>({"directory", "image"}) + "/jpeg";
            auto imageFile = std::ifstream(imagePath + "/" + splitUrl.back());
            std::ostringstream imageStream;
            imageStream << imageFile.rdbuf();
            auto imageString = imageStream.str();

            response.setStartLine({"HTTP/1.1", http_parser::response::StatusCode::ok});

            response.setHeader({"Content-Type", "image/jpeg"});
            response.setHeader({"Content-Length", std::to_string(imageString.length())});

            response.setbody(imageString);
        }
    );

    auto cssHandler = boost::make_shared<SyncRouteHandler>(
        "/css/{fileName}",
        http_parser::HttpMethod::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto& config = ConfigProvider::getConfig();
            auto splitUrl = http_parser::splitPath(request.startLine.url.getPath());
            auto cssPath = config.getValue<std::string>({"directory", "css"});
            auto cssFile = std::ifstream(cssPath + "/" + splitUrl.back());
            std::ostringstream cssStream;
            cssStream << cssFile.rdbuf();
            auto cssString = cssStream.str();

            response.setStartLine({"HTTP/1.1", http_parser::response::StatusCode::ok});

            response.setHeader({"Content-Type", "text/css; charset=utf-8"});
            response.setHeader({"Content-Lenght", std::to_string(cssString.length())});

            response.setbody(cssString);
        }
    );

    auto localesHandler = boost::make_shared<SyncRouteHandler>(
        "/locales/{fileName}",
        http_parser::HttpMethod::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){
            auto& config = ConfigProvider::getConfig();
            std::string localesPath = config.getValue<std::string>({"directory","locales"});
            auto splitPath = http_parser::splitPath(request.startLine.url.getPath());
            auto localeFile = std::ifstream(localesPath + "/" + splitPath.back() + ".json");
            
            std::ostringstream localesStream;
            localesStream << localeFile.rdbuf();
            auto localesStr = localesStream.str();

            response.setStartLine({"HTTP/1.1", http_parser::response::StatusCode::ok});

            response.setHeader({"Content-Type", "text/json; charset=utf-8"});
            response.setHeader({"Content-Lenght", std::to_string(localesStr.length())});

            response.setbody(localesStr);
        }
    );


    mult.AddRoute("/",http_parser::HttpMethod::GET,handler);
    mult.AddRoute("/go/go", http_parser::HttpMethod::GET, async_handler);
    mult.AddRoute("/{fileName}", http_parser::HttpMethod::GET, staticHandler);
    mult.AddRoute("/js/{fileName}", http_parser::HttpMethod::GET, jsHandler);
    mult.AddRoute("/components/{fileName}", http_parser::HttpMethod::GET, htmlTemplateHandler);
    mult.AddRoute("/image/svg/{fileName}", http_parser::HttpMethod::GET, imageSvgHandler);
    mult.AddRoute("/image/jpeg/{fileName}", http_parser::HttpMethod::GET, imageJpegHandler);
    mult.AddRoute("/css/{fileName}", http_parser::HttpMethod::GET, cssHandler);
    mult.AddRoute("/locales/{filename}", http_parser::HttpMethod::GET, localesHandler);
    

    auto& config = ConfigProvider::getConfig();
    server::HttpServer server(io,mult,config.getValue<int>({"server", "port"}));
    
    server.run();
    
   return 0;
}