#ifndef HTTP_URL_HEAD
#define HTTP_URL_HEAD

#include <string>
#include <unordered_map>
#include <vector>

namespace http_parser{


    inline std::vector<std::string> splitPath(std::string_view path) {
        std::vector<std::string> segments;
        
        if (path.empty()) {
            return segments;
        }
        if (path == "/") {
            segments.emplace_back("");
            return segments;
        }

        size_t startSegment = 0;
        const size_t pathLenght = path.length();

        if (path[0] == '/') {
            segments.emplace_back("");
            startSegment = 1;
        }

        while (startSegment < pathLenght) {
            const size_t endSegment = path.find('/', startSegment);
            
            if (endSegment == std::string_view::npos) {
                segments.emplace_back(path.substr(startSegment));
                break;
            }

            if (endSegment > startSegment) {
                segments.emplace_back(path.substr(startSegment, endSegment - startSegment));
            }

            startSegment = endSegment + 1;
        }

        return segments;
    }

    class Url {

    public:
        Url(){};
        
        Url& operator=(const Url& other){
            this->url_ = other.url_;
            this->fragment_= other.fragment_;
            this->query_ = other.query_;
            this->path_ = other.path_;
            return * this;
        }

        void setUrl(const std::string& url){
            url_=url;
        }

        void setPath(const std::string& path){
            path_=path;
        }

        void setQuery(const std::unordered_map<std::string, std::string>& query){
            query_=query;
        }

        void setFragment(const std::string& fragment){
            fragment_=fragment;
        }

        const std::string getUrl() const{
            return url_;
        }

        const std::string getPath() const{
            return path_;
        }

        const std::unordered_map<std::string,std::string> getQuery() const {
            return query_;
        }

        const std::string getFragment() const{
            return fragment_;
        };
    private:
        std::string url_;
        std::string path_;
        std::string fragment_;
        std::unordered_map<std::string,std::string> query_;
    };

}

#endif