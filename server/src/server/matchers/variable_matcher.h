#ifndef VARIABLE_MATCHER_HEAD
#define VARIABLE_MATCHER_HEAD

#include "matcher.h"

class VariableMatcher: public Matcher{

public:
    VariableMatcher(const std::string& varible);

    bool checkMatch(const std::string& variable_segment) override;

private:
    std::string variable_;

};


#endif