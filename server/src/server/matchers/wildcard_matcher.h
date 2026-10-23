#ifndef WILDCARDMATCHER
#define WILDCARDMATCHER

#include <string>

#include "matcher.h"

class WildcardMatcher: public Matcher {
 public:
    WildcardMatcher(const std::string& pattern);
    bool checkMatch(const std::string& path_segment) override;
 
 private:
    std::string pattern_;

};

#endif //WILDCARDMATCHER