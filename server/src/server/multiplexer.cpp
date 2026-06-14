#include "multiplexer.h"
#include <exception>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "request_error_exception.h"
#include "route_handler.h"
#include "../http_parser/http_response.h"
#include "../http_parser/http_request_parser.h"
#include "../http_parser/http_url.h"
#include "matchers/variable_matcher.h"
#include "matchers/wildcard_matcher.h"
#include <memory> 
#include <stdexcept> 

Multiplexer::RoutePtr Multiplexer::GetRoute(const std::string& path, http_parser::HttpMethod::Method method) {
    bool isPathMatch = false;
    std::vector<http_parser::HttpMethod::Method> methods;
    auto splitPathVec = http_parser::splitPath(path);

    for( const auto& handler: handlers_ ) {
        auto matchers = std::get<0>(handler);
        size_t matcherInd = 0;
        size_t splitStrInd = 0;
        isPathMatch = true;

        while(splitStrInd < splitPathVec.size()){
            
            if(matcherInd>matchers.size()-1){
                isPathMatch=false;
                break;
            }

            if(boost::dynamic_pointer_cast<WildcardMatcher>(matchers[matcherInd])){
                size_t remainingMatchers = matchers.size() - matcherInd -1;
                size_t maxEat = splitPathVec.size() - splitStrInd - remainingMatchers;
                if(maxEat == 0){
                    isPathMatch=true;
                    matcherInd++;
                }
                bool wildcardMatched = false;
                for(size_t eat = 0; eat< maxEat; eat++){
                    if(matchers[matcherInd]->checkMatch(splitPathVec[splitStrInd+eat])){
                        wildcardMatched = true;
                    } 
                    else{
                        wildcardMatched = false;
                        break;
                    }

                }
                if(wildcardMatched) {
                    isPathMatch=true;
                    splitStrInd+=maxEat;
                    matcherInd++;
                }
            }
            else if(!boost::dynamic_pointer_cast<WildcardMatcher>(matchers[matcherInd]) && 
                    matchers[matcherInd]->checkMatch(splitPathVec[splitStrInd])) {
                isPathMatch=true;
                matcherInd++;
                splitStrInd++;
            }
            else {
                isPathMatch=false;
                break;
            }
            
        }
        
        if(!isPathMatch){
            continue;
        }
        
        methods.push_back(std::get<3>(handler));
        
        if(std::get<3>(handler) == method){
            return std::get<1>(handler);
        }
    }
    
    if( isPathMatch ){
        std::string errorMessage;
        errorMessage += "This method" + http_parser::methodToString(method) + "is not supported on this server." + 
        "\n" + "Available methods of " + path + " is: ";
        
        for(const auto& method: methods){
            errorMessage += http_parser::methodToString(method) + ", ";
        }
        
        errorMessage.erase(errorMessage.size() - 3, 2);
        throw RequestErrorExtension(errorMessage, http_parser::response::StatusCode::method_not_allowed);
    }
    else{
        std::string errorMessage("This file is not existed.");
        throw RequestErrorExtension( errorMessage, http_parser::response::StatusCode::not_found);
    }
    
    return nullptr;
}

void Multiplexer::AddRoute(const std::string& path, http_parser::HttpMethod::Method method, RoutePtr route) {
    auto splitPathVec = http_parser::splitPath(path);
    std::vector<MatcherPrt> matcherVec;
    bool hasVaribleMatcher = false;
    bool hasWildcardMathcer = false;
    
    for(auto splitPath: splitPathVec){

        if(splitPath[0]=='{'){
            if(hasVaribleMatcher){
                throw std::invalid_argument("Shood be only one variable argument");
            }
            matcherVec.push_back(MatcherPrt(new VariableMatcher(splitPath.substr(1,splitPath.length()-2))));
            hasVaribleMatcher = true;
        }
        else if(splitPath=="**"){
            if(hasWildcardMathcer){
                throw std::invalid_argument("Shood be only one wildcard argument");
            }
            matcherVec.push_back(MatcherPrt(new WildcardMatcher("**")));
            hasWildcardMathcer = true;
        }
        else{
            matcherVec.push_back(MatcherPrt(new PathMatcher(splitPath)));
        }

    }

    for(size_t handlerInd=0; handlerInd < handlers_.size(); handlerInd++) {
        auto muxPath = std::get<2>(handlers_[handlerInd]);

        if(muxPath == path){
            auto muxMethod = std::get<3>(handlers_[handlerInd]);

            if(muxMethod == method){

                handlers_.erase(std::next(handlers_.begin(),handlerInd));
            
            }

        }

    }

    handlers_.push_back(std::make_tuple(matcherVec, route, path, method));
}

void Multiplexer::AddStaticRoute(const std::string& path, const std::string& fileSystemPath){
    auto staticFileRouter = boost::make_shared<SyncRouteHandler>(
        "/{fileName}",
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

    auto jsFileRoute = boost::make_shared<SyncRouteHandler>(
        "/js/{fileName}", 
        http_parser::HttpMethod::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){

        }
    );

    auto imageFileRoute = boost::make_shared<SyncRouteHandler>(
        "image/{fileName}",
        http_parser::HttpMethod::GET,
        [](http_parser::HttpRequest& request, http_parser::HttpResponse& response){

        }
    );
}


void Multiplexer::RemoveRoute(const std::string& path, http_parser::HttpMethod::Method method) {

    for(size_t handlerInd = 0; handlerInd < handlers_.size(); handlerInd++) {
        auto muxPath = std::get<2>(handlers_[handlerInd]);
        
        if(muxPath == path){
            auto maxMethod = std::get<3>(handlers_[handlerInd]);
            
            if(maxMethod == method){
                handlers_.erase(std::next(handlers_.begin(),handlerInd));
            
            }    
        }
    
    }
}