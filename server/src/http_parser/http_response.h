#ifndef HTTP_REQUEST_HEAD
#define HTTP_REQUEST_HEAD


#include <string>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace http_parser{
    namespace response{

        enum StatusCode{
            contin = 100,
            switching_protocols = 101,
            processing = 102,
            early_hints = 103,
            ok = 200,
            created = 201,
            accepted = 202,
            non_authoritative_information = 203,
            no_content = 204,
            reset_content = 205,
            partial_content = 206,
            multi_status = 207,
            already_reported = 208,
            IM_used = 226,
            multiple_choices = 300,
            moved_permanently = 301,
            found = 302,
            see_other = 303,
            not_modified = 304,
            use_proxy = 305,
            switch_proxy = 306,
            temporary_redirect = 307,
            permanent_redirect  = 308,
            bad_request = 400,
            unauthorized = 401,
            rayment_required = 402,
            forbidden = 403,
            not_found = 404,
            method_not_allowed = 405,
            not_acceptable = 406,
            proxy_authentification_required = 407,
            request_timeout = 408,
            conflict = 409,
            gone = 410,
            internal_server_error = 500,
            not_implemented = 501,
            bad_gateway = 502,
            service_unavailable = 503,
            gateway_timeout = 504,
            http_version_not_supported = 505,
            variant_also_negotiates = 506,
            insufficient_storage = 507,
            loop_detected = 508,
            not_extended = 510
        };
    }
    
    const std::unordered_map<response::StatusCode,const std::string> statusCode ={
        {response::StatusCode::contin, "continue"},
        {response::StatusCode::switching_protocols, "switching protocols"},
        {response::StatusCode::processing, "processing"},
        {response::StatusCode::early_hints,"early hints"}, 
        {response::StatusCode::ok,"OK"},
        {response::StatusCode::created,"created"},
        {response::StatusCode::accepted, "accepted"},
        {response::StatusCode::non_authoritative_information, "non authoritative information"},
        {response::StatusCode::no_content, "no content"},
        {response::StatusCode::reset_content, "reset content"},
        {response::StatusCode::partial_content, "partial content"},
        {response::StatusCode::multi_status, "multi status"},
        {response::StatusCode::already_reported, "already reported"},
        {response::StatusCode::IM_used, "IM used"},
        {response::StatusCode::multiple_choices, "multiple choices"},
        {response::StatusCode::moved_permanently, "moved permanently"},
        {response::StatusCode::found, "found"},
        {response::StatusCode::see_other, "see other"},
        {response::StatusCode::not_modified, "not modified"},
        {response::StatusCode::found, "found"},
        {response::StatusCode::see_other, "see other"},
        {response::StatusCode::not_modified, "not modified"},
        {response::StatusCode::use_proxy, "use proxy"},
        {response::StatusCode::switch_proxy, "switch proxy"},
        {response::StatusCode::temporary_redirect, "temporary redirect"},
        {response::StatusCode::permanent_redirect, "permanent redirect"},
        {response::StatusCode::bad_request, "bad request"},
        {response::StatusCode::unauthorized, "unauthorized"},
        {response::StatusCode::rayment_required, "rayment required"},
        {response::StatusCode::forbidden, "forbidden"},
        {response::StatusCode::not_found, "not found"},
        {response::StatusCode::method_not_allowed,"method not allowed"},
        {response::StatusCode::not_acceptable, "not acceptable"},
        {response::StatusCode::proxy_authentification_required, "proxy authentification required"},
        {response::StatusCode::request_timeout, "request timeout"},
        {response::StatusCode::conflict,"conflict"},
        {response::StatusCode::gone, "gone"},
        {response::StatusCode::internal_server_error, "internal server error"},
        {response::StatusCode::not_implemented, "not implemented"},
        {response::StatusCode::bad_gateway, "bad gateway"},
        {response::StatusCode::service_unavailable, "service unavailable"},
        {response::StatusCode::gateway_timeout, "gateway timeout"},
        {response::StatusCode::http_version_not_supported, "http version not supported"},
        {response::StatusCode::variant_also_negotiates, "variant also negotiates"},
        {response::StatusCode::insufficient_storage, "insufficient storage"},
        {response::StatusCode::loop_detected, "loop detected"},
        {response::StatusCode::not_extended, "not extended"}
    };

    class HttpResponseStart{
        public:

        HttpResponseStart() = default;
        HttpResponseStart(std::string version, response::StatusCode statusCode);
        std::string getStartString();

        private:
        std::string version_;
        response::StatusCode statusCode_;
    };

    struct Header{
        std::string name;
        std::string value;
    };

    class HttpResponse{
    public:

        HttpResponse() = default;
        HttpResponse(HttpResponseStart startLine, std::vector<Header> headers, std::string body);

        std::string toString();
        void setStartLine(const HttpResponseStart& startLine);
        void setStartLine(const std::string& version, const response::StatusCode& status);
        void setHeader(const Header& header);
        void setbody(const std::string& body);
    private:

        HttpResponseStart startLine_;
        std::vector<Header> headers_;
        std::string body_;
    };

}

#endif