#include "path_matcher.h"

PathMatcher::PathMatcher(const std::string& path): path_(path){

}

bool PathMatcher::checkMatch(const std::string& pathCondidate){
    return path_==pathCondidate;
}