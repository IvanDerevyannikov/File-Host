#include "storage_paths.h"

#include<system_error>
#include<stdexcept>

StoragePaths::StoragePaths(std::filesystem::path root)
    : root_(std::move(root)) {}

void StoragePaths::ensureDirectories() const {

    std::vector<std::filesystem::path> dirs = {
        root_,
        uploads(),
        tmp(),
    };

    for(const auto& dir: dirs) {
        std::error_code ec;

        std::filesystem::create_directories(dir,ec);
        
        if(ec) {
            throw std::runtime_error(
                "Cannot create directory " + dir.string() + ": " + ec.message()
            );
        }

        if (std::filesystem::exists(dir) && !std::filesystem::is_directory(dir)) {
            throw std::runtime_error(dir.string() + " exists but is not a directory");
        }
    }

}

