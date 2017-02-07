#ifndef HANDLER_DEFINED
#define HANDLER_DEFINED

#include <boost/asio.hpp>
#include "http_response.h"
#include <vector>

class handler {
public:
    // pure virtual function for handling a request
    virtual http_response handle_request() = 0;
};

class echo_handler : public handler {
public:
    echo_handler(std::vector<char>& request);
    http_response handle_request();
private:
    std::vector<char> to_send;
};

class static_handler : public handler {
public:
    static_handler(std::string curr_url);
    http_response handle_request();

private:
    std::string url;

    // gets path from url string
    std::string get_path_from_url(std::string url);
    // returns path to exec on current system
    std::string get_exec_path();
    // true if file exists
    bool file_exists(std::string filename);
    // returns content type header based on file extension
    std::string get_content_type(std::string filename);
    // reads raw file into vector of characters
    std::vector<char> read_file(std::string filename);
};

#endif
