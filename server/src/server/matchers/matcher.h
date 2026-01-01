#ifndef MATCHER_HEAD
#define MATCHER_HEAD


#include<string>


class Matcher{

public:
    virtual bool checkMatch(const std::string & path_segment) = 0;
    virtual ~Matcher(){}
};

#endif