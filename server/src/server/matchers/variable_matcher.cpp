#include "variable_matcher.h"

VariableMatcher::VariableMatcher(const std::string& variable): variable_(variable){}

bool VariableMatcher::checkMatch(const std::string& variable_segment){
    return !variable_segment.empty();
}