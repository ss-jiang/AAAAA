#include <sstream>
#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "../config_parser/config_parser.h"
//#include <gmock/gmock.h>
//#include "googletest/googlemock/include/gmock/gmock.h"
#include "../session.h"
#include "../server.h"
#include "../utils.h"
//using ::testing::AtLeast;

//tests that handle_write can successfully handle and error
/*TEST(HandleAcceptTest, handle_accept_err) {
  boost::asio::io_service io_service_;
  boost::asio::io_service io_service;
  server s(io_service_, 5050);
  session ss(io_service);
  boost::system::error_code error(4, boost::system::system_category());
  EXPECT_EQ(-1, s.handle_accept(&ss, error));
  //MockSession* session = new MockSession(io_service_);

  //EXPECT_CALL(*session, start()).Times(AtLeast(1));

}*/

