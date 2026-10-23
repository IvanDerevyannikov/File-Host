#include "file_service.h"

#include <filesystem>
#include <system_error>
#include <boost/filesystem.hpp>

#include "../config/config_provider.h"
#include "../utils/time_utils.h"

FileService::FileService(StoragePaths paths)
    : paths_(std::move(paths)) {}

Result<std::vector<FileInfo>> FileService::list() {
    //TODO take paths_ from app context not from config
    // const auto path = paths_.uploads();
    const std::filesystem::path path =
        std::filesystem::path(ConfigProvider::getConfig().getValue<std::string>({"storage", "root"})) / "uploads";

    std::error_code ec;
    if (!std::filesystem::is_directory(path, ec)) {
        return ErrorResponse{"uploads directory not found", 500};
    }

    std::vector<FileInfo> fileInfos;
    for (const auto& entry : std::filesystem::directory_iterator(path, ec)) {
        if (ec) {
            return ErrorResponse{ec.message(), 500};
        }

        // boost::filesystem::path filepath(entry.path().filename().string());

        FileInfo info;
        info.id = entry.path().stem().string();
        info.name = entry.path().filename().string();
        info.size = entry.is_regular_file() ? entry.file_size() : 0;
        info.extension = entry.path().extension().string();
        info.isDirectory = entry.is_directory();
        auto updateTime = std::filesystem::last_write_time(entry.path(), ec);

        if (!ec) {
            info.updatedAt = toIso8601(updateTime);
        }
        // boost::system::error_code ec;
        // std::time_t createTime = boost::filesystem::creation_time(filepath, ec);
        // info.createdAt = std::ctime(&createTime);
        // boost::system::error_code boostEC;

        fileInfos.push_back(std::move(info));
    }

    return fileInfos;
}