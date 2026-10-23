#ifndef ERROR_DTO_HEAD
#define ERROR_DTO_HEAD

#include <boost/json.hpp>

#include "../services/file_service.h"

inline boost::json::object errorToJson(const ErrorResponse& error) {
    return boost::json::object {
        {"error", error.httpCode},
        {"message", error.message},
    };
}


#endif  //ERROR_DTO_HEAD