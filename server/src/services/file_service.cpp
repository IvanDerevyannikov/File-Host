#include "file_service.h"

#include <filesystem>
#include <system_error>

FileService::FileService(StoragePaths paths)
    : paths_(std::move(paths)) {}

Result<std::vector<FileInfo>> FileService::list() {
    const auto path = paths_.uploads();

    std::error_code ec;
    if (!std::filesystem::is_directory(path, ec)) {
        return ErrorResponse{"uploads directory not found", 500};
    }

    std::vector<FileInfo> fileInfos;
    for (const auto& entry : std::filesystem::directory_iterator(path, ec)) {
        if (ec) {
            return ErrorResponse{ec.message(), 500};
        }

        FileInfo info;
        info.id = entry.path().stem().string();
        info.name = entry.path().filename().string();
        info.size = entry.is_regular_file() ? entry.file_size() : 0;
        info.contentType = entry.path().extension().string();
        info.isDirectory = entry.is_directory();

        fileInfos.push_back(std::move(info));
    }

    return fileInfos;
}