#include <sstream>
#include <string>
#include <iostream>
#include <map>

#include "gtest/gtest.h"
#include "../config_parser/config_parser.h"
#include "../session.h"
#include "../utils.h"

// TEST(BufferCreationTest, setup_obuffer) {
//   boost::asio::io_service io_service;
//   std::map<std::string, std::string> functions;
//   session s(io_service, functions);
// 
//   boost::asio::streambuf out_streambuf;
//   size_t sample_size = 5;
// 
//   s.setup_obuffer(out_streambuf, sample_size);
// 
//   std::string test_string( (std::istreambuf_iterator<char>(&out_streambuf)), std::istreambuf_iterator<char>() );
// 
//   EXPECT_EQ(test_string, 
//     "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 1\r\n\r\n");
// }
// 
// //tests that handle_write can successfully handle and error
// TEST(HandleWriteTest, handle_write_err) {
//   boost::asio::io_service io_service;
//   std::map<std::string, std::string> functions;
// 
//   session s(io_service, functions);
// 
//   boost::system::error_code error(4, boost::system::system_category());
//   size_t bytes_transferred = 0;
// 
//   EXPECT_EQ(-1, s.handle_write(error, bytes_transferred));
// }
