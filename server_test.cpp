#include <sstream>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "config_parser/config_parser.h"
#include <gmock/gmock.h>
//#include "googletest/googlemock/include/gmock/gmock.h"
#include "session.h"
#include "server.h"
#include "utils.h"

//tests that handle_write can successfully handle and error
TEST(HandleAcceptTest, handle_accept_err) {
  boost::asio::io_service io_service_;
  session* s = new session(io_service_);

  boost::asio::io_service io_service;
  server sv(io_service);
  
  MockTurtle turle;
  boost::system::error_code error(4, boost::system::system_category());

  EXPECT_CALL(turtle, sv.handle_accept(s, error)).Times(0);

}

