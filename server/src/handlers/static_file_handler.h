#ifndef STATIC_FILE_HANDLER_HEAD
#define STATIC_FILE_HANDLER_HEAD

#include <string>
#include <vector>
#include <functional>

#include "../server/multiplexer.h"
#include "../http_parser/http_request.h"
#include "../http_parser/http_response.h"
#include "../utils/fileUtils.h"



class StaticFileHandlerFabrics{


private:

    static Multiplexer::RoutePtr create(
        const std::string& path,
        const std::vector<std::string>& configPath,
        http_parser::HttpMethod::Method method,
        const std::string& contentType
    );

    static Multiplexer::RoutePtr createWildcardPathHandler(
        const std::string& path,
        const std::vector<std::string>& configPath,
        http_parser::HttpMethod::Method method,
        const std::string& contentTypes
    );

public:

    static Multiplexer::RoutePtr htmlHandler();
    static Multiplexer::RoutePtr jsHandler();
    static Multiplexer::RoutePtr cssHandler();
    static Multiplexer::RoutePtr htmlTemplateHandler();
    static Multiplexer::RoutePtr svgHandler();
    static Multiplexer::RoutePtr jpegHandler();
    static Multiplexer::RoutePtr localesHandler();

};



#endif //STATIC_FILE_HANDLER_HEAD