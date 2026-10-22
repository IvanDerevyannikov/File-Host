#ifndef FILE_SERVICE_HEAD
#define FILE_SERVICE_HEAD

#include <variant>
#include <string>
#include <vector>

#include "../server/storage/storage_paths.h"

struct FileInfo {
    std::string id;
    std::string name;
    std::uintmax_t size = 0;
    std::string contentType;
    bool isDirectory;
};


template <typename T>
struct CorrectResponse {
    T value;
};

struct ErrorResponse {
    std::string message;
    int httpCode;
};

template <typename T>
using Result = std::variant<T,ErrorResponse>;

class FileService{

public:
    FileService(StoragePaths paths);

    Result<std::vector<FileInfo>> list();
    Result<FileInfo> fileInfo(const std::string& fileName);
    Result<void> remove(const std::string& fileName);


private:
    StoragePaths paths_;
};

#endif //FILE_SERVICE_HEAD