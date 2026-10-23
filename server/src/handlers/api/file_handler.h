#ifndef FILE_HANDLER_HEAD
#define FILE_HANDLER_HEAD

#include "../../server/multiplexer.h"
#include "../../http_parser/http_request.h"
#include "../../http_parser/http_response.h"
#include "../../services/file_service.h"

class FileHandler{
 public:
    explicit FileHandler();

    Multiplexer::RoutePtr getFilePath();
    Multiplexer::RoutePtr postFilePath();
    Multiplexer::RoutePtr getFiles();
    Multiplexer::RoutePtr getFile();
    Multiplexer::RoutePtr postFile();

 private:
    FileService fileService_;
};


#endif // FILE_HANDLER_HEAD