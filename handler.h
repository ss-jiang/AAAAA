#ifndef HANDLER_DEFINED
#define HANDLER_DEFINED

#include <boost/asio.hpp>
#include "http_response.h"
#include "http_request.h"
#include "config_parser/config_parser.h"
#include <vector>

// Represents the parent of all request handlers. Implementation
// is long lived and created at server constrution.
class RequestHandler {
 public:
  enum Status {
    OK = 0
    // Define your status codes here.
  };

  // Initializes the handler. Returns a response code indicating success or
  // failure condition.
  // uri_prefix is the value in the config file that this handler will run for.
  // config is the contents of the child block for this handler ONLY.
  virtual Status Init(const std::string& uri_prefix,
                      const NginxConfig& config) = 0;

  // Handles an HTTP request, and generates a response. Returns a response code
  // indicating success or failure condition. If ResponseCode is not OK, the
  // contents of the response object are undefined, and the server will return
  // HTTP code 500.
  virtual Status HandleRequest(const Request& request,
                               Response* response) = 0;
};

// class handler {
// public:
//     // pure virtual function for handling a request
//     virtual Response handle_request() = 0;
// };

// class echo_handler : public handler {
// public:
//     echo_handler(std::vector<char>& request);
//     Response handle_request();
// private:
//     std::vector<char> to_send;
// };

// class static_handler : public handler {
// public:
//     static_handler(std::string curr_url);
//     Response handle_request();

// private:
//     std::string url;

//     // gets path from url string
//     std::string get_path_from_url(std::string url);
//     // returns path to exec on current system
//     std::string get_exec_path();
//     // true if file exists
//     bool file_exists(std::string filename);
//     // returns content type header based on file extension
//     std::string get_content_type(std::string filename);
//     // reads raw file into vector of characters
//     std::vector<char> read_file(std::string filename);
// };

#endif
