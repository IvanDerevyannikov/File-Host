#include "wildcard_matcher.h"

WildcardMatcher::WildcardMatcher(const std::string& pattern): pattern_(pattern){}

bool WildcardMatcher::checkMatch(const std::string& path){
    return true;
}