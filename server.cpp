#include "server.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
// Note: boost documentation referrenced to create server class

server::server(boost::asio::io_service& io_service, short port,
    std::map <std::string, std::pair<std::string, std::shared_ptr<RequestHandler>>> handler_map)
  : io_service_(io_service),
    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    handler_map_(handler_map)
{
  // create new session
  session* new_session = new session(io_service_, handler_map_);

  // TODO: change this to be input from config
  num_threads_ = 5;

  // accept new connection
  acceptor_.async_accept(new_session->socket(),
      boost::bind(&server::handle_accept, this, new_session,
      boost::asio::placeholders::error));
}

void server::run() {
  // Create a pool of threads to run all of the io_services.
  std::vector<boost::shared_ptr<boost::thread> > threads;
  for (std::size_t i = 0; i < num_threads_; ++i)
  {
    boost::shared_ptr<boost::thread> thread(new boost::thread(
      boost::bind(&boost::asio::io_service::run, &io_service_)));
    threads.push_back(thread);
  }

  // Wait for all threads in the pool to exit.
  for (std::size_t i = 0; i < threads.size(); ++i)
    threads[i]->join();
}

void server::handle_accept(session* new_session,
    const boost::system::error_code& error)
{
  if (!error)
  {
    new_session->start();
    // initiates the new accept operation
    new_session = new session(io_service_, handler_map_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&server::handle_accept, this, new_session,
        boost::asio::placeholders::error));
  }
  else
  {
    delete new_session;
  }
}
