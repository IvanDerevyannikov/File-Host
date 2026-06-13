cmake_minimum_required(VERSION 3.12)

project(asynchronous_http_server1 VERSION 1.0)

# Установка переменных для сборки
set(SOURCES src/async_http_server.cpp)

set(HTTP_PARSER_SOURCES
    src/http_parser/http_request.cpp
    src/http_parser/http_request_parser.cpp
    src/http_parser/http_response.cpp
)

set(HTTP_CONFIG
    src/config/config.h
    src/config/config.cpp
    src/config/config_provider.h
    src/config/config_provider.cpp
)

set(HTTP_PARSER_HEADERS
    src/http_parser/http_request.h
    src/http_parser/http_request_parser.h
    src/http_parser/http_response.h
    src/http_parser/http_url.h
)

set(HTTP_MATCHERS_SOURCES
    src/server/matchers/path_matcher.cpp
    src/server/matchers/variable_matcher.cpp
)

set(HTTP_MATCHERS_HEADERS
    src/server/matchers/matcher.h
    src/server/matchers/path_matcher.h
    src/server/matchers/variable_matcher.h
)

set(HTTP_ERROR_HANDLERS_HEADERS
    src/server/error_handlers/error_handler_factory.h
    src/server/error_handlerserror_handler.h
    src/server/error_handler_manager.h
)

set(HTTP_ERROR_HANDLERS_SOURCES
    src/server/error_handlers/error_handler.cpp
    src/server/error_handlers/error_handler_manager.cpp
)

set(HTTP_NET_SOURCES
    src/server/server.cpp  
    src/server/request.cpp
    src/server/route_handler.cpp 
    src/server/multiplexer.cpp
)

set(HTTP_NET_HEADERS
    src/server/server.h  
    src/server/request.h
    src/server/request_error_exception.h
    src/server/route_handler.h
    src/server/route_handler_traits.h
    src/server/multiplexer.h
)

# Установка выходной директории
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Установка компилятора C++ - это Clang 14
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

# Установка стандарта C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(Boost 1.85.0 REQUIRED COMPONENTS system filesystem thread coroutine regex date_time json)
find_package(yaml-cpp REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES} ${HTTP_PARSER_SOURCES} ${HTTP_PARSER_HEADERS} ${HTTP_NET_SOURCES} ${HTTP_NET_HEADERS} ${HTTP_CONFIG} ${HTTP_MATCHERS_HEADERS} ${HTTP_MATCHERS_SOURCES} ${HTTP_ERROR_HANDLERS_HEADERS} ${HTTP_ERROR_HANDLERS_SOURCES})


target_link_libraries(${PROJECT_NAME} Boost::system Boost::filesystem Boost::thread Boost::date_time Boost::json uring yaml-cpp)

# set_target_properties(${PROJECT_NAME} PROPERTIES
# RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"

# )

# cmake_minimum_required(VERSION 3.12)

# project(asynchronous_http_server1 VERSION 1.0)


# file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
# file(GLOB_RECURSE HTTP_PARSER_SOURCES CONFIGURE_DEPENDS "src/http_parser/*.cpp")
# file(GLOB_RECURSE HTTP_CONFIG CONFIGURE_DEPENDS "src/config/*.cpp")
# file(GLOB_RECURSE HTTP_MATCHERS_SOURCES CONFIGURE_DEPENDS "src/server/matchers/*.cpp")
# file(GLOB_RECURSE HTTP_ERROR_HANDLERS_SOURCES CONFIGURE_DEPENDS "src/server/error_handlers/*.cpp")
# file(GLOB_RECURSE HTTP_NET_SOURCES CONFIGURE_DEPENDS "src/server/*.cpp")


# file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
#     "src/*.h" 
#     "src/http_parser/*.h" 
#     "src/config/*.h" 
#     "src/server/*.h" 
#     "src/server/matchers/*.h" 
#     "src/server/error_handlers/*.h"
# )

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)


# set(CMAKE_CXX_COMPILER clang++)
# set(CMAKE_C_COMPILER clang)


# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)


# find_package(Boost 1.85.0 REQUIRED COMPONENTS system filesystem thread coroutine regex date_time json)
# find_package(yaml-cpp REQUIRED)

# add_executable(
#     ${PROJECT_NAME} 
#     ${SOURCES} 
#     ${HTTP_PARSER_SOURCES} 
#     ${HTTP_PARSER_HEADERS} 
#     ${HTTP_NET_SOURCES} 
#     ${HTTP_NET_HEADERS} 
#     ${HTTP_CONFIG} 
#     ${HTTP_MATCHERS_HEADERS} 
#     ${HTTP_MATCHERS_SOURCES} 
#     ${HTTP_ERROR_HANDLERS_HEADERS} 
#     ${HTTP_ERROR_HANDLERS_SOURCES}
# )


# target_link_libraries(${PROJECT_NAME} Boost::system Boost::filesystem Boost::thread Boost::date_time Boost::json uring yaml-cpp)

# set_target_properties(${PROJECT_NAME} PROPERTIES
# RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"

# )