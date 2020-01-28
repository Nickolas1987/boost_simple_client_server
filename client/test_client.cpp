#include "test_client.h"
namespace test_np{
    TestClient::TestClient(io_service& service, const std::string& address, int port,
                                                const std::function<void (TalkToServer::ptr, const std::string&)>& recv_callback, const std::function<bool (const std::string&)>& check) : 
                                                                                                                                    service_ref_(service), 
                                                                                                                                    recv_callback_(recv_callback),
                                                                                                                                    check_completed_(check){
        connection_ = TalkToServer::new_(service_ref_, ip::tcp::endpoint(ip::address::from_string(address.c_str()), port), this);
        connection_->start();
    }
    TestClient::~TestClient(){
    }
    void   TestClient::write(const std::string& s){
        connection_->doWrite(s);
    }
    void TestClient::stop(){
        connection_->stop();
    }
    void TestClient::processData(TalkToServer::ptr c, const std::string& m){
        if (recv_callback_){
            recv_callback_(c, m);
        }
    }
    size_t TestClient::isDataComplete(const std::string& data){
        bool found = check_completed_ ? check_completed_(data) : true;
        return found ? 0 : 1;
    }
};