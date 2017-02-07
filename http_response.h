#ifndef REQUEST_DEFINED
#define REQUEST_DEFINED

#include <string>
#include <vector>

class http_response {

public:
    http_response();
    // returns string representation of response for sending
    std::string to_string();
    void set_status(std::string &status);
    void add_header(std::string &header);
    void set_body(std::vector<char> &body);

private:
    // HTTP headers from HTTP specification
    const std::string HTTP_VER = "HTTP/1.1";
    std::string status_code;
    // Note: we may need to differentiate between general/entity/... headers later
    std::vector<std::string> headers;
    std::vector<char> message_body;
};

#endif
