#ifndef FILE_UTILS_HEAD
#define FILE_UTILS_HEAD

#include <fstream>
#include <sstream>
#include <string>
#include <optional>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>




struct FileData {
    std::string data;
    size_t size;
};


inline std::optional<FileData> readFile(const std::string& path) {
    std::ifstream file(path);

    std::stringstream oss;
    oss << file.rdbuf();
    std::string content = oss.str();
    size_t size = content.length(); 
    return FileData{std::move(content), size};
}


inline void read_async_file_simple(boost::asio::stream_file& file, 
                                   boost::shared_ptr<std::vector<char>> buffer,
                                   std::function<void(const boost::system::error_code& code, size_t bytes_read)> callback) {

    file.async_read_some(
        boost::asio::buffer(*buffer),     
        [&buffer, &callback](const boost::system::error_code& ec, size_t bytes_read){
            if(!ec){
                std::cout << buffer->data();
                callback(ec, bytes_read);
            }
        }
    );
}

#endif //FILE_UTILS_HEAD