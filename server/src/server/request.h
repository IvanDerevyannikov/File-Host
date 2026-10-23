#ifndef REQUEST_HEAD
#define REQUEST_HEAD

#include <iostream>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "multiplexer.h"
#include "error_handlers/error_handler_factory.h"
#include "error_handlers/error_handler_manager.h"
#include "../http_parser/http_request.h"
#include "../http_parser/http_response.h"

using boost::asio::ip::tcp;
namespace server{
    
    class Request : public boost::enable_shared_from_this<Request> {
     public:
        explicit Request(boost::asio::io_context& io, Multiplexer& mult, boost::shared_ptr<ErrorHandlerManager> errorHandlManagerPtr);
        void answer();
        boost::shared_ptr<tcp::socket> socket;

     private:
        void handleRead();
        void handleWrite();
        void handleError(const RequestErrorExtension& error);

        boost::asio::io_context& io_;
        boost::asio::streambuf request_;
        http_parser::HttpRequest req_;
        http_parser::HttpResponse res_;
        Multiplexer& mult_;
        std::array<char,126000> buffer_;
        boost::shared_ptr<ErrorHandlerManager> errorHandlManagerPtr_;
    };
}
#endif  //REQUEST_HPP