#include "session.h"
#include "http_request.h"
#include "http_response.h"
#include "handler.h"

#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <memory>

session::session(boost::asio::io_service& io_service,
  std::map <std::string, std::string> function_mapping)
  : socket_(io_service), function_mapping(function_mapping) {}

tcp::socket& session::socket()
{
  return socket_;
}

void session::start()
{
  std::cout << "========= Starting session =========" << std::endl;

  boost::asio::async_read_until(socket_, buffer, "\r\n\r\n",
      boost::bind(&session::handle_request, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

int session::handle_request(const boost::system::error_code& error,
    size_t bytes_transferred){

  std::string message_request = convert_buffer();
  auto request = Request::Parse(message_request);

  // std::unique_ptr<handler> handler_ptr;

  // std::cout << "Function: " << function << std::endl;
  // if (!error) {

  //   if (function == "echo_dir") {
  //     handler_ptr = std::unique_ptr<handler>(new echo_handler(message_request));
  //   } else if (function == "static_dir") {
  //     handler_ptr = std::unique_ptr<handler>(new static_handler(url));
  //   }
  //   else {
  //     // TODO: Handle this error
  //     std::string status = "500 Internal Server Error";
  //     return -1;
  //   }

  //   http_response response = handler_ptr->handle_request();
  //   // write out response
  //   write_string(response.to_string());
  // }
  // else{
  //   std::cerr << error.message() << std::endl;
  //   return -1;
  // }
  write_string("TODO");
  return 0;

}

// given a string, writes out to socket and ends connection
void session::write_string(std::string send) {

  std::cout << "========= Writing out =========" << std::endl;

  // output buffer
  boost::asio::streambuf out_streambuf;
  std::ostream out(&out_streambuf);
  out << send;

  boost::asio::write(socket_, out_streambuf);

  // wait for transmission Note: this could hang forever
  tcdrain(socket_.native_handle());

  // close socket
  boost::system::error_code ec;
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  socket_.close();

  std::cout << "========= Ending Session =========" << std::endl;
}

std::string session::convert_buffer()
{
  std::string s{
    buffers_begin(buffer.data()),
    buffers_end(buffer.data())
  };

  return s;
}



std::string session::get_function_from_url(std::string url)
{
  std::string function = "";
  // TODO: log these errors
  if (url.length() == 0)
    return "Error: Blank Function Field";
  if (url.length() == 1 && url == "/")
    return "/";

  int second_slash_pos = url.find("/", 1);
  // string between first and second slashs
  std::string dir = url.substr(0, second_slash_pos);

  std::map<std::string, std::string>::iterator it = function_mapping.find(dir);
  // if valid function found
  if (it != function_mapping.end()){
     function = it->second;
  }
  else{
    // TODO: log these errors
    std::cerr << "Error in determining function" << std::endl;
    std::cerr << "URL causing error :" << url << std::endl;
  }
  return function;
}

