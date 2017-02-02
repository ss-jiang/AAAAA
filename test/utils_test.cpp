#include <string>
#include <iostream>

#include "gtest/gtest.h"
#include "../config_parser/config_parser.h"
#include "../session.h"
#include "../utils.h"

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
