#include "test_client.h"
namespace test_np{
    TestClient::TestClient(io_service& service, IConnectionHandler& handler, const std::string& address, int port) : service_ref_(service), connection_handler_(handler){
        connection_ = TalkToServer::new_(service_ref_, ip::tcp::endpoint(ip::address::from_string(address.c_str()), port), *this);
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
        connection_handler_.recvMsg(c, m);
    }
    size_t TestClient::isDataComplete(const std::string& data){
        bool found = connection_handler_.checkMsg(data);
        return found ? 0 : 1;
    }
};