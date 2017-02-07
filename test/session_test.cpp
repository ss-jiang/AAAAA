#include <sstream>
#include <string>
#include <iostream>
#include <map>

#include "gtest/gtest.h"
#include "../config_parser/config_parser.h"
#include "../session.h"
#include "../utils.h"

TEST(SessionTest, HandlerSwitching) {
  boost::asio::io_service io_service;
  std::map<std::string, std::string> functions;

  session s(io_service, functions);
  boost::system::error_code error;

  int error_num = s.handle_request(error, 10);
  EXPECT_EQ(error_num, -1);

  boost::system::error_code error_two(5, boost::system::system_category());
  error_num = s.handle_request(error_two, 10);
  EXPECT_EQ(error_num, -1);
}
