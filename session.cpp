#include "session.h"
#include <termios.h>

session::session(boost::asio::io_service& io_service)
  : socket_(io_service) {}

tcp::socket& session::socket()
{
  return socket_;
}

void session::start()
{
  std::cout << "========= Starting session =========" << std::endl;
  
  boost::asio::async_read_until(socket_, buffer, "\r\n\r\n",
      boost::bind(&session::handle_read, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void session::handle_read(const boost::system::error_code& error,
    size_t bytes_transferred)
{

  std::cout << "========= Writing out =========" << std::endl;

  // output buffer
  boost::asio::streambuf out_streambuf;
  std::ostream out(&out_streambuf);
  
  // setup headers
  out << "HTTP/1.1 200 OK\r\n";
  out << "Content-Type: text/plain\r\n";
  // minus 4 for the double carriage return
  out << "Content-Length: " << bytes_transferred - 4 << "\r\n\r\n";
  
  // echo message under headers
  out << &buffer;
  
  if (!error)
  {
    boost::asio::write(socket_, out_streambuf);
    
    // wait for transmission
    tcdrain(socket_.native_handle());

    // close socket
    boost::system::error_code ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    socket_.close();
    
    std::cout << "========= Ending Session =========" << std::endl;
  }
  else
  {
    delete this;
  }
}
