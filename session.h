#ifndef SESSION_DEFINED
#define SESSION_DEFINED

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <vector>
#include <map>

#include "http_response.h"

// for tcp type
using boost::asio::ip::tcp;

class session
{
public:
  session(boost::asio::io_service& io_service, std::map <std::string, std::string> function_mapping);
  tcp::socket& socket();
  void start();
  // sets up echo response object
  int handle_echo(const boost::system::error_code& error, size_t bytes_transferred, http_response& response);
  // sets up static response object
  int handle_static(std::string& url, http_response& response);
  // handles requests by seeing which type of response is required
  int handle_request(const boost::system::error_code& error, size_t bytes_transferred);
  // writes out a string to the socket and closes the connection
  void write_string(std::string send);

private:
  std::vector<char> convert_buffer();
  std::string get_function_from_url(std::string url);
  std::string get_path_from_url(std::string url);
  // returns path to exec on current system
  std::string get_exec_path();
  // true if file exists
  bool file_exists(std::string filename);
  // returns content type header based on file extension
  std::string get_content_type(std::string filename);
  // reads raw file into vector of characters
  std::vector<char> read_file(std::string filename);
  tcp::socket socket_;
  std::map <std::string, std::string> function_mapping;
  boost::asio::streambuf buffer;
  
};

#endif
