#ifndef REQUEST_DEFINED
#define REQUEST_DEFINED

#include <string>
#include <vector>

class Response {

public:
    enum ResponseCode {
        // Define your HTTP response codes here.
    };

    void SetStatus(const ResponseCode response_code);
    void AddHeader(const std::string& header_name, const std::string& header_value);
    void SetBody(const std::string& body);

    // returns string representation of response
    std::string ToString();

private:
    // HTTP headers from HTTP specification
    const std::string HTTP_VER = "HTTP/1.1";
    ResponseCode status_code;
    // Note: we may need to differentiate between general/entity/... headers later
    std::vector<std::pair<std::string, std::string>> headers;
    std::string message_body;
};

#endif
