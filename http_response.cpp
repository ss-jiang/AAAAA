#include "http_response.h"

std::string Response::ToString() {
    std::string res_str = "";

    // status header
    res_str += HTTP_VER;
    // TODO
    //res_str += " " + status_code + "\r\n";

    // TODO: headers
    // for(auto it = headers.begin(); it != headers.end(); ++it) {
    //     res_str += *it + "\r\n";
    // }

    // padding
    res_str += "\r\n";

    // body
    res_str += std::string(message_body.begin(), message_body.end());

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
