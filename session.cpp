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

  Response response;
  RequestHandler::Status status = handler_ptr->HandleRequest(*request, &response);

  // if handler failed, use NotFoundHandler
  if (status == RequestHandler::FAIL) {
    std::unique_ptr<RequestHandler> error_handler (new NotFoundHandler);
    error_handler->HandleRequest(*request, &response);
  }

  write_string(response.ToString());
  return 0;
}

// given a string, writes out to socket and ends connection
void session::write_string(std::string send) {
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
}

std::string session::get_message_request()
{
  std::string s{
    buffers_begin(buffer.data()),
    buffers_end(buffer.data())
  };

  return s;
}

//get the longest prefix that matches with what's specified in config. If no match is found, an empty string is returned
//for example: "/foo/bar" gets matched with "/foo/bar" before it gets matched with "/foo"
std::string session::get_longest_prefix(const std::string original_url)
{
  unsigned int longest_match_size = 0;
  std::string longest_match = "";

  // for each possible matching url
  for (auto const& iter : handler_map){
    // if prefix match
    if (original_url.find(iter.first) == 0){
      // save away longest match
      if (iter.first.length() > longest_match_size){
        longest_match = iter.first;
        longest_match_size = iter.first.length();
      }
    }
  }
  return longest_match;
}
