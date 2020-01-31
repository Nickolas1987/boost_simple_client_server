#pragma once
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>
#include <functional>
namespace test_np{
class Timer{
public:
    Timer(io_service& s, const std::function<void()>& c) : timer_(s), interval_(1000), callback_(c) {
    }
    void timeout(const boost::system::error_code &e) {
      if (e)
         return;
      callback_();
    }
    void reset(int i){
      interval_ = i;
      wait();
    }
    void cancel() {
      timer_.cancel();
    }
private:
    void wait() {
      timer_.expires_from_now(boost::posix_time::milliseconds(interval_)); 
      timer_.async_wait(boost::bind(&Timer::timeout, this, boost::asio::placeholders::error));
    }
    deadline_timer timer_;
    int interval_;
    std::function<void()> callback_;
};

}