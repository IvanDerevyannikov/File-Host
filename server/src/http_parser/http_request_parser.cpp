#include "http_request_parser.h"
#include <sstream>
#include <regex>
#include <exception>
#include <iostream>
#include <utility>
#include <vector>
#include <exception>
#include <iomanip>

namespace http_parser{

   HttpStartString parseStartLine(const std::string& startLine) {
        std::istringstream startLineStream(startLine);
        std::string methodToken, urlToken, versionToken;

        if (!(startLineStream >> methodToken >> urlToken >> versionToken)) {
            throw std::invalid_argument("Malformed start line: " + startLine);
        }

        HttpStartString startLineData;
        startLineData.method = HttpMethod(methodToken);

        const std::regex urlPattern(
            R"(^(https?:\/\/)?((localhost(:\d+)?|[\da-z\.-]+)(\.[a-z\.]{2,6})?)?(\/[\w\.\-\/]*)*(\?([\w\-]+=[\w\-]*&?)*)?\/?$)"
        );
        if (!std::regex_match(urlToken, urlPattern)) {
            throw std::invalid_argument("Invalid URL format: " + urlToken);
        }

        startLineData.url = parseUrl(urlToken);
        startLineData.version = versionToken;

        return startLineData;
    }


    http_parser::Url parseUrl(const std::string& urlStr){
        auto url = http_parser::Url();
        auto usrStrIndex=0;

        size_t fragmentIndex = urlStr.find("#");
        size_t queryIndex = urlStr.find("?");
        if(fragmentIndex!=std::string::npos && fragmentIndex>queryIndex){
            url.setFragment(urlStr.substr(fragmentIndex+1));
            url.setPath(urlStr.substr(0, fragmentIndex));
        }
        else if(fragmentIndex<queryIndex){
            url.setFragment(urlStr.substr(fragmentIndex+1, queryIndex-1));
            url.setPath(urlStr.substr(0, fragmentIndex));
        }
        else{
            url.setPath(urlStr);
        }

        if(queryIndex!=std::string::npos && queryIndex<fragmentIndex){
            size_t queryEnd = (fragmentIndex != std::string::npos) ? fragmentIndex : urlStr.length();
            std::string queryStr = urlStr.substr(queryIndex + 1, queryEnd - queryIndex - 1);
                
            url.setQuery(parseUrlQuery(queryStr));
            url.setPath(urlStr.substr(0, queryIndex));
        }
        else if(queryIndex>fragmentIndex){
            size_t queryEnd = urlStr.length();
            std::string queryStr = urlStr.substr(queryIndex+1, queryEnd - queryIndex - 1);
            url.setQuery(parseUrlQuery(queryStr));
            url.setPath(urlStr.substr(0,fragmentIndex));
        }

        return url;
    }

    std::unordered_map<std::string, std::string> parseUrlQuery(const std::string& queryString){
        std::unordered_map<std::string, std::string> query;
        std::vector<std::string> queryVec;
        size_t startQuery=0;
        size_t endQuery = queryString.find("&");
        while(endQuery!=std::string::npos){
            queryVec.push_back(queryString.substr(startQuery, endQuery-startQuery));
            startQuery=endQuery+1;
            endQuery= queryString.find("&", startQuery);
        }
        queryVec.push_back(queryString.substr(startQuery));
        
        for(auto queryParameter: queryVec){
            size_t parameterEnd = queryParameter.find("=");
            if(parameterEnd==std::string::npos){
                //throw std::exception("Bad query value");
            }
            std::string keyStr = decodeQuery(queryParameter.substr(0,parameterEnd));
            std::string valueStr = decodeQuery(queryParameter.substr(parameterEnd+1));

            query.emplace(keyStr, valueStr);
        }
        return query;
    }

    std::string decodeQuery(const std::string& queryParameter){
        std::ostringstream decoded;
        for(size_t i=0;i<queryParameter.length(); ++i){
            if(queryParameter[i] == '%' && i+2 < queryParameter.length()){
                int hex;
                std::istringstream hexStream(queryParameter.substr(i+1,2));
                if(hexStream >> std::hex >> hex){
                    decoded << static_cast<char>(hex);
                    i+=2;
                }
                else{
                    decoded << queryParameter[i];
                }
            }
            else if(queryParameter[i]=='+'){
                decoded << ' ';
            }
            else{
                decoded << queryParameter[i];
            }
        }
        return decoded.str();
    }

    std::string encodeQuery(const std::string& queryParameter){
        std::ostringstream encoded;
        encoded.fill('0');
        encoded << std::hex;
        for(auto c: queryParameter){
            if(std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~'){
                encoded << c;
            }
            else{
                encoded << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
            }
        }
        return encoded.str();
    }

    bool isIdempotent(const HttpMethod::Method& method) {
        switch (method) {
            case HttpMethod::Method::GET:
            case HttpMethod::Method::HEAD:
            case HttpMethod::Method::PUT:
            case HttpMethod::Method::DELETE:
            case HttpMethod::Method::OPTIONS:
            case HttpMethod::Method::TRACE:
                return true;
            default:
                return false;
        }
    }

    std::pair<std::string, std::string> parseHeader(const std::string& headerLine) {
        auto delimiterPos = headerLine.find(':');
        if (delimiterPos == std::string::npos) {
            throw std::invalid_argument("Invalid header format: " + headerLine);
        }

        auto key = headerLine.substr(0, delimiterPos);
        auto valueStart = headerLine.find_first_not_of(" ", delimiterPos + 1);
        auto valueEnd = headerLine.find_last_not_of("\r");

        if (valueStart == std::string::npos || valueEnd == std::string::npos) {
            throw std::invalid_argument("Malformed header value: " + headerLine);
        }

        auto value = headerLine.substr(valueStart, valueEnd - valueStart + 1);
        return {std::move(key), std::move(value)};
    }

    HttpRequest parseHttpRequest(const std::string& rawRequest) {
        HttpRequest request;
        std::istringstream requestStream(rawRequest);

        // Parse the start line
        std::string startLine;
        if (!std::getline(requestStream, startLine)) {
            throw std::invalid_argument("Request does not contain a start line.");
        }
        request.startLine = parseStartLine(startLine);

        // Parse headers
        std::string headerLine;
        while (std::getline(requestStream, headerLine) && headerLine != "\r") {
            request.headers.emplace(parseHeader(headerLine));
        }

        // Parse body if present
        if (requestStream.peek() != EOF) {
            std::string bodyContent((std::istreambuf_iterator<char>(requestStream)),
                                    std::istreambuf_iterator<char>());
            request.body = std::move(bodyContent);
        }

        return request;
    }

    std::string methodToString(const http_parser::HttpMethod::Method& method){
        if(method == HttpMethod::GET) {return "GET";}
        else if(method == HttpMethod::HEAD) {return "HEAD";}
        else if(method == HttpMethod::POST) {return "POST";}
        else if(method == HttpMethod::PUT) {return "PUT";}
        else if(method == HttpMethod::DELETE) {return  "DELETE";}
        else if(method == HttpMethod::CONNECT) {return  "CONNECT";}
        else if(method == HttpMethod::OPTIONS) {return  "OPTIONS";}
        else if(method == HttpMethod::TRACE) {return "TRACE";}
        else if(method == HttpMethod::PATCH) {return  "PATCH";}
        return "GET";
    }

    std::vector<std::string> parseHeaderValue(const std::string& value, const std::string& delimeterSymbols){
        std::vector<std::string> parsedValue;
        size_t start = 0;
        size_t commaPos = value.find(delimeterSymbols);
        
        while (commaPos != std::string::npos) {  
            parsedValue.push_back(value.substr(start, commaPos - start));
            start = commaPos + 1;
            if (start == std::string::npos) break;
            if(value[start] == ' ') start+=1;
            commaPos = value.find(delimeterSymbols, start);
        }
        if (start != std::string::npos && start < value.length()) {
            parsedValue.push_back(value.substr(start));
        }
        
        return parsedValue;
    }

    // std::vector<std::string> parseHeaderSemicolon(const std::string& value){
        
    // }
}