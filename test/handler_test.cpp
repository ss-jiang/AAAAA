#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

#include "gtest/gtest.h"
#include "../handler.h"


TEST(EchoHandlerTest, EchoClassTest) {
  std::unique_ptr<EchoHandler> handler(new EchoHandler);
  std::string str_req = "GET /echo HTTP/1.1\r\ntest\r\n\r\n";

  auto req = Request::Parse(str_req);

  Response res;
  RequestHandler::Status s = handler->HandleRequest(*req, &res);

  std::string expected = "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(str_req.size() - 4) +"\r\nContent-Type: text/plain\r\n\r\n" + str_req;
  EXPECT_EQ(res.ToString(), expected);
  EXPECT_EQ(s, RequestHandler::PASS);
}

TEST(NotFoundHandlerTest, NotFoundClassTest) {
  std::unique_ptr<NotFoundHandler> handler(new NotFoundHandler);
  Request req;
  Response res;
  RequestHandler::Status s = handler->HandleRequest(req, &res);

  std::string body = "404 NOT FOUND";
  std::string expected = "HTTP/1.1 404 Not found\r\nContent-Length: " + std::to_string(body.size()) +"\r\nContent-Type: text/plain\r\n\r\n" + body;
  EXPECT_EQ(res.ToString(), expected);
  EXPECT_EQ(s, RequestHandler::PASS);
}


// TEST(StaticHandlerTest, StaticClassTest){
//   std::string url = "/public/index.html";
//   static_handler handle(url);

//   http_response r = handle.handle_request();
//   EXPECT_EQ(r.to_string(), "HTTP/1.1 200 OK\r\nContent-Length: 28\r\nContent-Type: text/html\r\n\r\n<head> Hello World! </head>\n");
// }

