#include "talk_to_server.h"
#include "itest_client.h"
#include <iostream>

namespace test_np{
    TalkToServer::TalkToServer(io_service& service_ref, const ip::tcp::endpoint& ep, ITestClient* p) : ep_(ep), parent_(p), sock_(service_ref), started_(false) {
    }
    TalkToServer::~TalkToServer(){
        stop();
    }
    void TalkToServer::start() {
        sock_.async_connect(ep_, boost::bind(&TalkToServer::onConnect, this, _1));
        started_ = true;
    }
    TalkToServer::ptr TalkToServer::new_(io_service& service_ref, const ip::tcp::endpoint& ep, ITestClient* p) {
        TalkToServer::ptr res(new TalkToServer(service_ref, ep, p));
        return res;
    }
    void TalkToServer::stop() {
        if ( !started_) {
            return;
        }
        started_ = false;
        sock_.close();
    }
    bool TalkToServer::started() const { 
        return started_; 
    }
    ip::tcp::socket & TalkToServer::sock() { 
        return sock_;
    }
    void TalkToServer::onRead(const error_code & err, size_t bytes) {
        if ( err) {
          stop();
        }
        if ( !started() ){
          return;
        }

        if (parent_){
          std::string msg = read_buffer_;
          parent_->processData(shared_from_this(), msg);
        }
    }
    void TalkToServer::onWrite(const error_code & err, size_t bytes) {
    }
    void TalkToServer::onConnect(const error_code & err) {
        if (err){
          stop();
        }
        doRead();
    }
    void TalkToServer::doRead(bool need_clear) {
        if (need_clear){
          read_buffer_.clear();
          read_buffer_.resize(0);
        }
        async_read(sock_, dynamic_buffer(read_buffer_), 
                   boost::bind(&TalkToServer::readComplete, shared_from_this(), _1, _2), boost::bind(&TalkToServer::onRead, this, _1, _2));
    }
    void TalkToServer::doWrite(const std::string & msg) {
        if ( !started() ){ 
            return;
        }
        write_buffer_ = msg;
        sock_.async_write_some( buffer(&write_buffer_[0], msg.size()), 
                                boost::bind(&TalkToServer::onWrite, this, _1, _2));
    }
    size_t TalkToServer::readComplete(const boost::system::error_code & err, size_t bytes) {
        if ( err){
          return 0;
        }
        bool res =  parent_ ? parent_->isDataComplete(read_buffer_) : 0;
        return res;
    }
}