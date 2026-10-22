#include "./file_handler.h"
#include "../../config/config_provider.h"

FileHandler::FileHandler()
    : fileService_(StoragePaths(
          ConfigProvider::getConfig().getValue<std::string>({"storage", "root"})))
{
}