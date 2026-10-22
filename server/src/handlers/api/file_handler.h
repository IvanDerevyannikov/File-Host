#ifndef FILE_HANDLER_HEAD
#define FILE_NANDLER_HEAD

#include "../../server/multiplexer.h"
#include "../../http_parser/http_request.h"
#include "../../http_parser/http_response.h"

class FileHandler{

public:
    Multiplexer::RoutePtr getFilePath();
    Multiplexer::RoutePtr postFilePath();
    Multiplexer::RoutePtr getFiles();
    Multiplexer::RoutePtr getFile();
    Multiplexer::RoutePtr postFile();
};


#endif // FILE_HANDLER_HEAD