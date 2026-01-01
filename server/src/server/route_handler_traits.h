#ifndef ROUTE_HANDLER_HEAD
#define ROUTE_HANDLER_HEAD

#include <functional>
#include <type_traits>
#include "../http_parser/http_request.h"

template <typename T, typename = void>
struct has_handler: std::false_type{};

template <typename T>
struct has_handler<T, std::void_t(decltype(std::declval<T>().Handle(std::declval<http_parser::HttpRequest>())))>
  :std::true_type{};


template<typename T, typename = void>
struct has_async_handler : std::false_type{};

template <typename T>
struct has_async_handle<T, std::void_t<decltype(std::declval<T>().AsyncHandle(
    std::declval<http_parser::HttpRequest&>(), std::declval<std::function<void()>>()))>> 
    : std::true_type {};

    
#endif