#include <iostream>
#include <sstream>
#include <ctime>
#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <yaml-cpp/yaml.h>

#include "./server/multiplexer.h"
#include "./server/route_handler.h"
#include "./server/server.h"
#include "./config/config.h"
#include "./config/config_provider.h"
#include "./routes/static_routes.h"

using boost::asio::ip::tcp;

class HttpServer;



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


// void read_async_file_simple(boost::asio::io_context& io, std::string& fileRes) {
//     auto file = std::make_shared<boost::asio::stream_file>(
//         io, 
//         "big_file.txt",  // просто имя, т.к. файл в той же папке
//         boost::asio::file_base::read_only
//     );
// }


void read_async_file_simple(boost::asio::io_context& io, std::string& fileRes) {
    auto file = std::make_shared<boost::asio::stream_file>(
        io, 
        "/home/ivan/Programs/boost_asio_client_server_example/server/big_file.txt", 
        boost::asio::file_base::read_only
    );
    
    auto buffer = std::make_shared<std::vector<char>>(20 * 1024);
    
    auto read_callback = std::make_shared<std::function<void(const boost::system::error_code&, size_t)>>();
    *read_callback = [file, buffer, &fileRes, read_callback](
        const boost::system::error_code& error, size_t bytes_read) {
        std::cout << "RES SIZE AFTER READ        " << fileRes.size() << "HERE" << "\n\n\n\n";
        if (error == boost::asio::error::eof) {
            std::cout << "RES SIZE AFTER READ\n\n\n\n\n" << fileRes << "HERE" << "\n\n\n\n";
            return;
        }
        
        if (error) {
            return;
        }
        
        fileRes.append(buffer->data(), bytes_read);
        
        file->async_read_some(
            boost::asio::buffer(buffer->data(), buffer->size()),
            *read_callback
        );
    };
    
    file->async_read_some(
        boost::asio::buffer(buffer->data(), buffer->size()),
        *read_callback
    );
}

int main (int argc, char *argv[])
{
   std::cout << "Boost version: " << BOOST_LIB_VERSION << std::endl;
   boost::asio::io_context io;
   std::cout << getGmtTime() << '\n';

    Multiplexer mult;
    std::string res;
    read_async_file_simple(io, res);
    registerStaticRoutes(mult);
    auto& config = ConfigProvider::getConfig();
    server::HttpServer server(io,mult,config.getValue<int>({"server", "port"}));
    std::cout << res.size() << "       RES SIZE HERE:!@!#!$!$!@" <<"\n" << "\n";
    server.run();
    
   return 0;
}