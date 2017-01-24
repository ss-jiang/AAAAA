#ifndef SESSION_DEFINED
#define SESSION_DEFINED

#include <boost/bind.hpp>
#include <boost/asio.hpp>

// for tcp type
using boost::asio::ip::tcp;

class session
{
public:
  session(boost::asio::io_service& io_service);
  tcp::socket& socket();
  void start();
  void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
  void handle_write(const boost::system::error_code& error);

private:
  tcp::socket socket_;
  enum { max_length = 1024 };
  char data_[max_length];
};

#endif
