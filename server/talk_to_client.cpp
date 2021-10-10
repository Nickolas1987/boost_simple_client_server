#include "talk_to_client.h"
#include "itest_server.h"

namespace test_np{
    TalkToClient::TalkToClient(io_service& ref, ITestServer& p) : service_ref_(ref), parent_(p), sock_(service_ref_), started_(false) {
    }
    TalkToClient::~TalkToClient(){
        stop();
    }
    void TalkToClient::start() {
        started_ = true;
        doRead();
    }
    TalkToClient::ptr TalkToClient::new_(io_service& s, ITestServer& p) {
        TalkToClient::ptr res(new TalkToClient(s, p));
        return res;
    }
    void TalkToClient::stop() {
        if ( !started_) {
            return;
        }
        started_ = false;
        sock_.close();

        TalkToClient::ptr self = shared_from_this();
        parent_.removeClient(self);
    }
    bool TalkToClient::started() const { 
        return started_; 
    }
    ip::tcp::socket & TalkToClient::sock() { 
        return sock_;
    }
    void TalkToClient::onRead(const error_code & err, size_t bytes) {
        if ( err) {
          stop();
        }
        if ( !started() ){
          return;
        }
        // process the msg
        std::string msg = read_buffer_;
        parent_.processData(shared_from_this(), msg);
    }
    void TalkToClient::onWrite(const error_code & err, size_t bytes) {
    }
    void TalkToClient::doRead(bool need_clear) {
        if (need_clear){
          read_buffer_.clear();
          read_buffer_.resize(0);
        }
        async_read(sock_, dynamic_buffer(read_buffer_), 
                   boost::bind(&TalkToClient::readComplete, this, _1, _2), boost::bind(&TalkToClient::onRead, this, _1, _2));
    }
    void TalkToClient::doWrite(const std::string & msg) {
        if ( !started() ){ 
            return;
        }
        write_buffer_ = msg;
        sock_.async_write_some( buffer(&write_buffer_[0], msg.size()), 
                                boost::bind(&TalkToClient::onWrite, this, _1, _2));
    }
    size_t TalkToClient::readComplete(const boost::system::error_code & err, size_t bytes) {
        if ( err){
          return 0;
        }
        bool res =  parent_.isDataComplete(read_buffer_);
        return res;
    }
}