#ifndef FILE_DTO_HEAD
#define FILE_DTO_HEAD

#include <boost/json.hpp>

#include "../services/file_service.h"

//TODO namespace and function name
inline boost::json::object toJson(const FileInfo& file) {
    return boost::json::object {
        {"id", file.id},
        {"name", file.name},
        {"size", file.size},
        {"extension", file.extension},
        {"isDirectory", file.isDirectory},
        {"updatedAt", file.updatedAt},
        {"createdAt", file.createdAt},
    };
}


inline boost::json::object listResponse(const std::vector<FileInfo>& files) {
    boost::json::array filesArray;
    filesArray.reserve(files.size());
    size_t totalSize =0;
    for(const auto& file: files) {
        filesArray.push_back(toJson(file));
        totalSize+=file.size;
    }

    return boost::json::object {
        {"files", std::move(filesArray)},
        {"total", files.size()},
        {"totalSize", totalSize}
    };
}


#endif //FILE_DTO_HEAD