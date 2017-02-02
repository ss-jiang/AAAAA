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
      boost::bind(&session::handle_write, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}


int session::handle_write(const boost::system::error_code& error,
    size_t bytes_transferred)
{

  std::cout << "========= Writing out =========" << std::endl;

  // output buffer
  boost::asio::streambuf out_streambuf;
  
  //sets up the output buffer with the correct headers
  setup_obuffer(out_streambuf, bytes_transferred);
  
  if (!error)
  {
    boost::asio::write(socket_, out_streambuf);
    
    // wait for transmission Note: this could hang forever
    tcdrain(socket_.native_handle());

    // close socket
    boost::system::error_code ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    socket_.close();
    
    std::cout << "========= Ending Session =========" << std::endl;
  }
  else
  {
    std::cerr << error.message() << std::endl;
    return -1;
  }
  return 0;
}

void session::setup_obuffer(boost::asio::streambuf& out_streambuf, size_t bytes_transferred)
{
  std::ostream out(&out_streambuf);
  // setup headers
  out << "HTTP/1.1 200 OK\r\n";
  out << "Content-Type: text/plain\r\n";
  // minus 4 for the double carriage return
  out << "Content-Length: " << bytes_transferred - 4 << "\r\n\r\n";
  
  // echo message under headers
  out << &buffer;
}
