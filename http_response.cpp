#include "http_response.h"

http_response::http_response() {
    status_code = "200 OK";
}

std::string http_response::to_string() {
    std::string res_str = "";

    // status header
    res_str += HTTP_VER;
    res_str += " " + status_code + "\r\n";

    // headers
    for(auto it = headers.begin(); it != headers.end(); ++it) {
        res_str += *it + "\r\n";
    }
    
    // padding
    res_str += "\r\n";

    // body
    res_str += std::string(message_body.begin(), message_body.end());
    
    return res_str; 
}

void http_response::set_status(std::string &status) {
    status_code = status;
}

void http_response::add_header(std::string &header) {
    headers.push_back(header);
}

void http_response::set_body(std::vector<char> &body) {
    message_body = body;
}
