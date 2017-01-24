#ifndef SERVER_DEFINED
#define SERVER_DEFINED

#include <boost/asio.hpp>
#include "session.h"

// for tcp type
using boost::asio::ip::tcp;

class server {

public:
  server(boost::asio::io_service& io_service, short port);
  void handle_accept(session* new_session, const boost::system::error_code& error);

private:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
};

#endif
