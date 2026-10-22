cmake_minimum_required(VERSION 3.12)
project(asynchronous_http_server1 VERSION 1.0)


file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HTTP_PARSER_SOURCES CONFIGURE_DEPENDS "src/http_parser/*.cpp")
file(GLOB_RECURSE HTTP_CONFIG CONFIGURE_DEPENDS "src/config/*.cpp")
file(GLOB_RECURSE HTTP_MATCHERS_SOURCES CONFIGURE_DEPENDS "src/server/matchers/*.cpp")
file(GLOB_RECURSE HTTP_ERROR_HANDLERS_SOURCES CONFIGURE_DEPENDS "src/server/error_handlers/*.cpp")
file(GLOB_RECURSE HTTP_NET_SOURCES CONFIGURE_DEPENDS "src/server/*.cpp")
file(GLOB_RECURSE HTTP_HANDLERS_SOURCES CONFIGURE_DEPENDS "src/handlers/*.cpp")
file(GLOB_RECURSE HTTP_ROUTES_SOURCES CONFIGURE_DEPENDS "src/routes/*.cpp")
file(GLOB_RECURSE HTTP_API_ROUTES_SOURCES CONFIGURE_DEPENDS "src/routes/api/*.cpp")


file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
    "src/*.h" 
    "src/http_parser/*.h" 
    "src/config/*.h" 
    "src/server/*.h" 
    "src/server/matchers/*.h" 
    "src/server/error_handlers/*.h"
    "src/utils/*.h"
    "src/handlers/*.h"
    "src/routes/*.h"
    "src/routes/api/*.h"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(Boost 1.85.0 REQUIRED COMPONENTS system filesystem thread coroutine regex date_time json)
find_package(yaml-cpp REQUIRED)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES} 
    ${HTTP_PARSER_SOURCES} 
    ${HTTP_CONFIG} 
    ${HTTP_MATCHERS_SOURCES} 
    ${HTTP_ERROR_HANDLERS_SOURCES}
    ${HTTP_NET_SOURCES}
    ${HTTP_HANDLERS_SOURCES}
    ${HTTP_ROUTES_SOURCES}
    ${HTTP_API_ROUTES_SOURCES}
    ${HEADER_FILES}
)


target_compile_definitions(${PROJECT_NAME} PRIVATE
    BOOST_ASIO_HAS_FILE=1
    BOOST_ASIO_HAS_IO_URING=1
)

add_compile_definitions(
    BOOST_ASIO_HAS_FILE=1
    BOOST_ASIO_HAS_IO_URING=1
)

target_link_libraries(${PROJECT_NAME} 
    Boost::system 
    Boost::filesystem 
    Boost::thread 
    Boost::date_time 
    Boost::json 
    uring 
    yaml-cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)