#include "server.h"

// Note: boost documentation referrenced to create server class

server::server(boost::asio::io_service& io_service, short port,
    std::map <std::string, std::pair<std::string, std::shared_ptr<RequestHandler>>> handler_map)
  : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
{
  // create new session
  session* new_session = new session(io_service_, handler_map);

  // accept new connection
  acceptor_.async_accept(new_session->socket(),
      boost::bind(&server::handle_accept, this, new_session,
        boost::asio::placeholders::error, handler_map));
}

void server::handle_accept(session* new_session,
    const boost::system::error_code& error,
    std::map <std::string, std::pair<std::string, std::shared_ptr<RequestHandler>>> handler_map)
{
  if (!error)
  {
    // on acceptance, start session
    new_session->start();
    new_session = new session(io_service_, handler_map);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
          boost::asio::placeholders::error, handler_map));
  }
  else
  {
    delete new_session;
  }
}
