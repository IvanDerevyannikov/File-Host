#ifndef MULTIPLEXER_HEAD
#define MULTIPLEXER_HEAD

#include <vector>
#include "route_handler.h"
#include "matchers/path_matcher.h"
#include "matchers/matcher.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <utility>
#include <functional>
#include <tuple>


class Multiplexer {
public:
    using RoutePtr = boost::shared_ptr<RouteHandler>;
    using MatcherPrt = boost::shared_ptr<Matcher>;
    using MatcherVec = std::vector<MatcherPrt>;
    using handlerInfo = std::tuple<MatcherVec, RoutePtr, std::string, http_parser::HttpMethod::Method>;
    using handlerVec = std::vector<handlerInfo>;

public:
    Multiplexer() = default;
    RoutePtr GetRoute(const std::string& path, http_parser::HttpMethod::Method method);
    void AddStaticRoute(const std::string& path, const std::string& fileSystemPath);
    void AddRoute(const std::string& path, http_parser::HttpMethod::Method method, RoutePtr route);
    void RemoveRoute(const std::string& path, http_parser::HttpMethod::Method method);
private:
    handlerVec handlers_;
};

#endif // MULTIPLEXER_HEAD