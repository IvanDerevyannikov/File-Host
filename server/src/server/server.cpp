#include "server.h"
#include "request.h"
//#include <boost/make_shared.hpp>

namespace server{

    HttpServer::HttpServer(boost::asio::io_context& io, Multiplexer& mult,size_t port): io_(io), mult_(mult),
        acceptor_(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port))) {
            auto errorHandlFactotyPtr= boost::shared_ptr<ErrorHandlerFactory>(new ErrorHandlerFactory());
            errorHandlFactotyPtr->initialize();
            errorHandlManagerPtr_= boost::shared_ptr<ErrorHandlerManager>(new ErrorHandlerManager(errorHandlFactotyPtr));
        }

    HttpServer::~HttpServer(){}

    void HttpServer::run(){
        startAccept();
        io_.run();
    }

    void HttpServer::startAccept(){
        boost::shared_ptr<Request> req(new Request(io_, mult_, errorHandlManagerPtr_));
        acceptor_.async_accept(*req->socket,
            [this, req](const boost::system::error_code& error) {
                if (!error) {
                    req->answer();
                }
                this->startAccept();
            });
    }

    void HttpServer::handleAccept(boost::shared_ptr<Request> request, const boost::system::error_code& error){
        if (!error) 
            request->answer();
        this->startAccept();
    }

}