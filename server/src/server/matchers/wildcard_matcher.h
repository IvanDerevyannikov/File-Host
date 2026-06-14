#ifndef WILDCARDMATCHER
#define WILDCARDMATCHER

#include "matcher.h"
#include <string>

class WildcardMatcher: public Matcher {
public:
    WildcardMatcher(const std::string& pattern);
    bool checkMatch(const std::string& path_segment) override;
private:
    std::string pattern_;

};

#endif //WILDCARDMATCHER