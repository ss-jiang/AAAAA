#include "http_response.h"

std::string Response::ToString() {
    std::string res_str = "";

    // status header
    res_str += HTTP_VER;
    std::string str_code;

    switch (status_code) {
        case OK:
            str_code = "200 OK";
            break;
        case NOT_FOUND:
            str_code = "404 Not found";
            break;
        default:
            str_code = "500 Internal server error";
            break;
    }
    res_str += " " + str_code + "\r\n";

    // headers
    for(auto it = headers.begin(); it != headers.end(); ++it) {
        res_str += (*it).first + " " + (*it).second + "\r\n";
    }

    // padding
    res_str += "\r\n";

    // body
    res_str += message_body;

    return res_str;
}

void Response::SetStatus(const ResponseCode response_code) {
    status_code = response_code;
}

void Response::AddHeader(const std::string& header_name, const std::string& header_value) {
    std::pair<std::string, std::string> header(header_name, header_value);
    headers.push_back(header);
}

void Response::SetBody(const std::string& body) {
    message_body = body;
}
