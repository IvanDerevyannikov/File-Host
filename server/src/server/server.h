#ifndef SERVER_HEAD
#define SERVER_HEAD

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "multiplexer.h"
#include "error_handlers/error_handler_factory.h"
#include "error_handlers/error_handler_manager.h"

namespace server{

    class Request;

    class HttpServer{
        friend class Request;
        public:
        HttpServer(boost::asio::io_service& io,Multiplexer& mult , size_t port);
        ~HttpServer();

        void run();

        private:
        void startAccept();
        void handleAccept(boost::shared_ptr<server::Request> request, const boost::system::error_code& error);

        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::io_service& io_;
        Multiplexer& mult_;
        boost::shared_ptr<ErrorHandlerManager> errorHandlManagerPtr_;
    };

}

#endif