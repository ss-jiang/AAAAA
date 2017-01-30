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
  int handle_write(const boost::system::error_code& error, size_t bytes_transferred);
  void setup_obuffer(boost::asio::streambuf& out_streambuf, size_t bytes_transferred);

private:
  tcp::socket socket_;
  boost::asio::streambuf buffer;
};

#endif
