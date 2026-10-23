#ifndef STORAGE_PATHS_HEAD
#define STORAGE_PATHS_HEAD

#include <filesystem>
#include <string>
#include <vector>

class StoragePaths{
 public:
    explicit StoragePaths(std::filesystem::path root);

    void ensureDirectories() const;

    const std::filesystem::path& root() const { return root_;}

    std::filesystem::path uploads() const { return root_ / "uploads";}
    std::filesystem::path tmp() const { return root_ / "tmp";}
    

 private:
    std::filesystem::path root_;
};

#endif //STORAGE_PATHS_HEAD