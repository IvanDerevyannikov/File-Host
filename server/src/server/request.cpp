#include <sstream>
#include <istream>

#include <iostream>
#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>

#include <boost/pointer_cast.hpp>
#include <boost/asio.hpp>
#include "../http_parser/http_response.h"
#include "../http_parser/http_request.h"
#include "../http_parser/http_request_parser.h"

#include "../config/config_provider.h"
#include "request.h"

namespace server{

    Request::Request(boost::asio::io_context& io, Multiplexer& mult, boost::shared_ptr<ErrorHandlerManager> errorHandlManagerPtr): 
        io_(io), mult_(mult), errorHandlManagerPtr_(errorHandlManagerPtr)
    {
        socket.reset(new boost::asio::ip::tcp::socket(io_));
    }

    void Request::answer(){
        if(!socket->is_open())
            return;
        handleRead();
    }

    void Request::handleRead(){
        auto self = shared_from_this();

        socket->async_read_some(boost::asio::buffer(buffer_),[this,self](boost::system::error_code errorCode, size_t n){
            if(!errorCode){
                try{
                    req_ = http_parser::parseHttpRequest(std::string(buffer_.data(),n));
                    std::cout << req_.startLine.url.getPath() << '\n';
                    auto handler = mult_.GetRoute(req_.startLine.url.getPath(), req_.startLine.method.getMethod());
                    if(boost::dynamic_pointer_cast<AsyncRouteHandler>(handler)!=nullptr){
                        std::cout << "async" << '\n';
                        handler->AssyncHandle(req_, res_, io_, [this](){
                            handleWrite();
                        });
                    }
                    else{
                        handler->Handle(req_, res_);
                        handleWrite();
                    }
                }
                catch(const RequestErrorExtension& error){
                    std::cout << "Request emit an error: " << error.what() << '\n';
                    handleError(error);
                }
            }

        });
    }

    void Request::afterRead( const boost::system::error_code& error, std::size_t bytesSended){

    }

    void Request::handleWrite(){
        auto self(shared_from_this());
        //std::cout << res_.toString();
        auto data = std::make_shared<std::string>(res_.toString());
        boost::asio::async_write(*socket, boost::asio::buffer(*data),[this,data,self](const boost::system::error_code& code, size_t n){

        });
    }

    void Request::handleError(const RequestErrorExtension& error){
        std::cout << error.what();
        errorHandlManagerPtr_->executeError(res_,req_,error.statusCode());
        handleWrite();
    }

}
