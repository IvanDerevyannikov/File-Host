#ifndef CONFIG_PROVIDER_HEAD
#define CONFIG_PROVIDER_HEAD

#include "config.h"
#include <memory>
#include <string>

class ConfigProvider {
public:
    static Config& getConfig() {
        static ConfigProvider instance("../.config");
        return *instance.configPtr_;
    }

private:
    ConfigProvider(const std::string& path) {
        if(configPtr_ == nullptr){
            configPtr_.reset(new Config(path));
        }
    }
    
    ConfigProvider(const ConfigProvider& other) = delete;
    ConfigProvider& operator=(const ConfigProvider& other) = delete;
    
    std::unique_ptr<Config> configPtr_;
};

#endif