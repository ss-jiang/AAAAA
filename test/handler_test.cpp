#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "../handler.h"


TEST(EchoHandlerTest, EchoClassTest) {
  std::string str = "Hello";
  std::vector<char>body(str.begin(), str.end());

  echo_handler e(body);
  http_response r = e.handle_request();

  EXPECT_EQ(r.to_string(), "HTTP/1.1 200 OK\r\nContent-Length: 1\r\nContent-Type: text/plain\r\n\r\nHello");
}

TEST(StaticHandlerTest, StaticClassTest){
  std::string url = "/public/index.html";
  static_handler handle(url);

  http_response r = handle.handle_request();
  EXPECT_EQ(r.to_string(), "HTTP/1.1 200 OK\r\nContent-Length: 28\r\nContent-Type: text/html\r\n\r\n<head> Hello World! </head>\n");
}



