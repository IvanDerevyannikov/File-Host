#include <iostream>
#include <sstream>
#include <ctime>
#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <yaml-cpp/yaml.h>

#include "./server/multiplexer.h"
#include "./server/route_handler.h"
#include "./server/server.h"
#include "./config/config.h"
#include "./config/config_provider.h"
#include "./routes/static_routes.h"
#include "./routes/api/file_routes.h"
#include "./server/storage/storage_paths.h"

using boost::asio::ip::tcp;

std::string getGmtTime(){
    boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
    auto pFacet = new boost::posix_time::time_facet("%a, %d %b %Y %H:%M:%S GMT");

    std::ostringstream oss;
    oss.imbue(std::locale(std::locale::classic(),pFacet));
    oss << now;
    return oss.str();
}

void read_async_file_simple(boost::asio::io_context& io, std::string& fileRes) {
    auto file = std::make_shared<boost::asio::stream_file>(
        io, 
        "/home/ivan/Programs/boost_asio_client_server_example/server/big_file.txt", 
        boost::asio::file_base::read_only
    );
    
    auto buffer = std::make_shared<std::vector<char>>(20 * 1024);
    
    auto read_callback = std::make_shared<std::function<void(const boost::system::error_code&, size_t)>>();
    *read_callback = [file, buffer, &fileRes, read_callback](
        const boost::system::error_code& error, size_t bytes_read) {
        std::cout << "RES SIZE AFTER READ        " << fileRes.size() << "HERE" << "\n\n\n\n";
        if (error == boost::asio::error::eof) {
            std::cout << "RES SIZE AFTER READ\n\n\n\n\n" << fileRes << "HERE" << "\n\n\n\n";
            return;
        }
        
        if (error) {
            return;
        }
        
        fileRes.append(buffer->data(), bytes_read);
        
        file->async_read_some(
            boost::asio::buffer(buffer->data(), buffer->size()),
            *read_callback
        );
    };
    
    file->async_read_some(
        boost::asio::buffer(buffer->data(), buffer->size()),
        *read_callback
    );
}

int main (int argc, char *argv[])
{
    std::cout << "Boost version: " << BOOST_LIB_VERSION << std::endl;
    boost::asio::io_context io;
    std::cout << getGmtTime() << '\n';

    auto& config = ConfigProvider::getConfig();

    const std::string root = config.getValue<std::string>({"storage", "root"});
    StoragePaths paths(root);
    paths.ensureDirectories();
    const auto path = paths.uploads();

    Multiplexer mult;
    registerStaticRoutes(mult);
    registerFileApiRoutes(mult);

    server::HttpServer server(io,mult,config.getValue<int>({"server", "port"}));
    server.run();
    
   return 0;
}