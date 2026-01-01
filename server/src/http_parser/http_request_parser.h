#ifndef HTTP_REQUEST_PARSER_HEAD
#define HTTP_REQUEST_PARSER_HEAD

#include "http_request.h"
#include "http_url.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace http_parser {

    HttpStartString parseStartLine(const std::string& startString);
    http_parser::Url parseUrl(const std::string& urlStr);
    std::unordered_map<std::string, std::string> parseUrlQuery(const std::string& queryString);
    std::string decodeQuery(const std::string& queryParameter);
    std::string encodeQuery(const std::string& queryParameter);
    HttpRequest parseHttpRequest(const std::string& request);
    bool isIdempotent(const HttpMethod::Method& httpMethod);
    std::pair<std::string,std::string> parseHeader(const std::string& headerString);
    std::string methodToString(const http_parser::HttpMethod::Method& method);
    std::vector<std::string> parseHeaderValue(const std::string& value, const std::string& delimeterSymbols);
    // std::vector<std::string> parseHeaderSemicolon(const std::string& value);
}

#endif