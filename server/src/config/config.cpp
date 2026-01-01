#include "config.h"
#include <fstream>
#include <iostream>

Config::Config(const std::string& path):path_(path){
    config_ = YAML::LoadFile(path_);
}

bool Config::reset(){
    config_.reset(YAML::LoadFile(path_));
    return config_.IsDefined();
}

bool Config::save(){
    std::ofstream configFile(path_);
    if(!configFile.is_open()){
        std::cerr << "File does not exist";
        return false;
    }
    configFile << config_;
    configFile.close();
    return true;
}