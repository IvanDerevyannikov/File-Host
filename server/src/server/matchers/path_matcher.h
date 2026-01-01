#ifndef PATH_MATCHER_HEAD
#define PATH_MATCHER_HEAD


#include <string>
#include "matcher.h"

class PathMatcher: public Matcher {

public:
    PathMatcher(const std::string& path);

    bool checkMatch(const std::string& pathCondidate) override;

private:
    std::string path_;  
};

#endif