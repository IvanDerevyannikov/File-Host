#ifndef CONFIG_HEAD
#define CONFIG_HEAD

#include <string>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <vector>

class ConfigProvider;

class Config {
    friend class ConfigProvider; // Дружественность должна быть в private секции

public:
    Config(const Config& other) = delete;
    Config(Config&& other) noexcept : config_(std::move(other.config_)), path_(std::move(other.path_)) {}
    
    Config& operator=(const Config& other) = delete;
    Config& operator=(Config&& other) noexcept {
        if (this != &other) {
            config_ = std::move(other.config_);
            path_ = std::move(other.path_);
        }
        return *this;
    }

    template<typename T>
    T getValueByKey(const std::string& key) const {
        return config_[key].as<T>();
    }
    
    template<typename T>
    T getValue(const std::vector<std::string>& keys) const {
        YAML::Node valNode = YAML::Clone(config_);
        for(int i=0 ; i<keys.size()-1; ++i) {
            YAML::Node nextNode = valNode[keys[i]].as<YAML::Node>();
            valNode = nextNode;
        }
        
        return valNode[keys[keys.size()-1]].as<T>();
    }

    template<typename T>
    void setValue(const std::string& key, const T& value) {
        config_[key] = value;
    }

    template<typename T>
    void setValue(const std::vector<std::string>& keys, const T& value) {
        YAML::Node valNode = config_[keys[0]];
        for(size_t i = 0; i < keys.size() - 1; ++i) {
            if(!valNode[keys[i]]) {
                valNode = YAML::Node(YAML::NodeType::Map);
            }
            valNode = valNode[keys[i + 1]];
        }
        valNode = value;
    }

    bool reset();
    bool save();
private:
    Config(const std::string& path);
    YAML::Node config_;
    std::string path_;
};

#endif