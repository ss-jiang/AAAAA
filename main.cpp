#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>

#include "config_parser/config_parser.h"
#include "server.h"
#include "session.h"

int main(int argc, char* argv[])
{
  try
  {
    // only one arg to server possible
    if (argc != 2)
    {
      std::cerr << "Usage: web-server <config-file>\n";
      return 1;
    }

    NginxConfigParser parser;
    NginxConfig config;
    
    // read config file
    parser.Parse(argv[1], &config);

    short port = -1;
    int num_statements = config.statements_.size();
    
    // look for port num in config
    for (int i = 0; i < num_statements; i++) {
        // if curr statement : port <num>
        if (config.statements_[i]->tokens_[0] == "port" 
            && config.statements_[i]->tokens_.size() == 2) {
            
            // save away port
            port = std::atoi(config.statements_[i]->tokens_[1].c_str());
        }
    }

    if (port == -1) {
        std::cerr << "Port number was not specified in config\n";
        return 1;
    }

    boost::asio::io_service io_service;
    
    // create and start server
    server s(io_service, port);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
