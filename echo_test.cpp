#include <sstream>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "config_parser/config_parser.h"
#include "session.h"
#include "utils.h"

TEST(BufferCreationTest, setup_obuffer) {
  boost::asio::io_service io_service;
  session s(io_service);

  boost::asio::streambuf out_streambuf;
  size_t sample_size = 5;

  s.setup_obuffer(out_streambuf, sample_size);

  std::string test_string( (std::istreambuf_iterator<char>(&out_streambuf)), std::istreambuf_iterator<char>() );

  EXPECT_EQ(test_string, 
    "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 1\r\n\r\n");
}

// tests the handle_write returns 0 in the case of success
// TEST(HandleWriteTest, handle_write_ok) {
//   boost::asio::io_service io_service;
//   session s(io_service);

//   boost::system::error_code error;
//   size_t bytes_transferred = 10;

//   short port = 8080;
//   tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), port));
//   acceptor_.accept(s.socket());

//   EXPECT_EQ(0, s.handle_write(error, bytes_transferred));
//   acceptor_.cancel();
//   acceptor_.close();

// }

//tests that handle_write can successfully handle and error
TEST(HandleWriteTest, handle_write_err) {
  boost::asio::io_service io_service;
  session s(io_service);

  boost::system::error_code error(4, boost::system::system_category());
  size_t bytes_transferred;

  EXPECT_EQ(-1, s.handle_write(error, bytes_transferred));
}

// test for setting up the server info struct in main.cpp
TEST(MainTest, setup_server_info_struct) {
  NginxConfigParser parser;
  NginxConfig config;
  char config_file[] = "simple_config";

  if (!parser.Parse(config_file, &config)) {
    std::cerr << "Error: malformed config file." << std::endl;
  }  

  ServerInfo info = utils::setup_info_struct(config);

  EXPECT_EQ(8080, info.port);
}
