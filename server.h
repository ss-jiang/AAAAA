#ifndef SERVER_DEFINED
#define SERVER_DEFINED

#include <boost/asio.hpp>
#include "session.h"
#include "handler.h"

#include <map>
#include <memory>

// for tcp type
using boost::asio::ip::tcp;

class server {

public:
  server(boost::asio::io_service& io_service, short port,
  	std::map<std::string, std::pair<std::string, std::shared_ptr<RequestHandler>>> function_mapping);

  void handle_accept(session* new_session, const boost::system::error_code& error);

  // spawns pool of threads to run io_service loop
  void run();

private:
  // number of threads
  size_t num_threads_;

  // io_service used to perform async operations
  boost::asio::io_service& io_service_;

  // acceptor used to listen for connections
  tcp::acceptor acceptor_;

  // handler map for all incoming requests
  std::map <std::string, std::pair<std::string, std::shared_ptr<RequestHandler>>> handler_map_;
};

#endif
