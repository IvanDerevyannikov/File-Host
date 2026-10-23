#ifndef FILE_ROUTES_HEAD
#define FILE_ROUTES_HEAD

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "../../server/multiplexer.h"
#include "../../handlers/api/file_handler.h"


inline void registerFileApiRoutes(Multiplexer& mult) {
    auto fileHandler = std::make_shared<FileHandler>();
    mult.AddRoute(
        "/api/v1/files", 
        http_parser::HttpMethod::GET, 
        fileHandler->getFiles()
    );
}


#endif // FILE_ROUTES_HEAD