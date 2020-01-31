#pragma once
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/core/noncopyable.hpp>
#include <vector>
using namespace boost::asio;


namespace test_np{

  class ITestClient;

  class TalkToServer : public boost::enable_shared_from_this<TalkToServer>
                     , boost::noncopyable {
      TalkToServer(io_service& service, const ip::tcp::endpoint&, ITestClient&);
    public:
      ~TalkToServer();
      typedef boost::system::error_code error_code;
      typedef boost::shared_ptr<TalkToServer> ptr;

      void start();
      static ptr new_(io_service& service_ref, const ip::tcp::endpoint&, ITestClient&);
      void stop();
      bool started() const ;
      ip::tcp::socket & sock() ;
      void doRead(bool need_clear = true);
      void doWrite(const std::string & msg);
    private:
      void onRead(const error_code & err, size_t bytes);
      void onWrite(const error_code & err, size_t bytes);
      void onConnect(const error_code & err);
      size_t readComplete(const boost::system::error_code & err, size_t bytes);
    private:
      ip::tcp::endpoint ep_;
      ITestClient& parent_;
      ip::tcp::socket sock_;
      std::string read_buffer_;
      std::string write_buffer_;
      bool started_;
  };
}
