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
  std::map <std::string, std::shared_ptr<RequestHandler>> handler_map)
  : socket_(io_service), handler_map(handler_map) {}

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

  auto request = Request::Parse(get_message_request());

  // Parse signifies error to outside function by wiping raw_request
  if (request->raw_request() == "") {
    std::cerr << "Error parsing the following message request:" << get_message_request() << std::endl;
    return -1;
  }

  // find longest prefix that uniquely matches uri (our key for handler_map)
  std::string longest_prefix = get_longest_prefix(request->uri());
  // get corresponding handler
  std::shared_ptr<RequestHandler> handler_ptr = handler_map[longest_prefix];

  // Response response;

  // // TODO: this needs to be set the handler specified in the config by "default" (store in info struct?)
  // // if no matching handler_ptr, then set to NotFoundHandler
  // if (handler_ptr == NULL){
  //   handler_ptr = std::shared_ptr<RequestHandler>(new NotFoundHandler());
  // }

  // handler_ptr->HandleRequest(*request, &response);

  // write_string(response.ToString());
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

std::string session::get_message_request()
{
  std::string s{
    buffers_begin(buffer.data()),
    buffers_end(buffer.data())
  };

  return s;
}

//removes the part of url that specifies the handler, leaving just the file path
std::string session::resetUri(const std::string original_uri, const std::string longest_prefix){
  //set uri to uri - longest_prefix
  std::string new_uri = original_uri.substr(longest_prefix.length());
  //remove the leading / for corner case of "/" being mapped to StaticHandler
  if (new_uri[0] == '/')
    new_uri = new_uri.substr(1);
  return new_uri;
}

//get the longest prefix that matches with what's specified in config. If no match is found, an empty string is returned
//for example: "/foo/bar" gets matched with "/foo/bar" before it gets matched with "/foo"
std::string session::get_longest_prefix(const std::string original_url)
{
  unsigned int longest_match_size = 0;
  std::string longest_match = "";
  for (auto const& iter : handler_map){
    if (original_url.find(iter.first) == 0){
      if (iter.first.length() > longest_match_size){
        longest_match = iter.first;
        longest_match_size = iter.first.length();
      }
    }
  }
  return longest_match;
}
