#include "test_server.h"
namespace test_np{
    TestServer::TestServer(io_service& service, int port, const std::function<void (TalkToClient::ptr)>& accept_callback, 
                                                          const std::function<void (TalkToClient::ptr, const std::string&)>& recv_callback, 
                                                          const std::function<bool (const std::string&)>& check) : service_ref_(service), 
                                                                                                                         acceptor_(service, ip::tcp::endpoint(ip::tcp::v4(), port)),
                                                                                                                         accept_callback_(accept_callback), recv_callback_(recv_callback),
                                                                                                                         check_completed_(check){
        auto start_client_ = TalkToClient::new_(service_ref_, this);
        addClient(start_client_);
        acceptor_.async_accept(start_client_->sock(), boost::bind(&TestServer::handleAccept, this, start_client_,_1));
    }
    TestServer::~TestServer(){
    }
    void TestServer::handleAccept(TalkToClient::ptr client, const boost::system::error_code & err){
        client->start();
        if(accept_callback_){
            accept_callback_(client);
        }
        TalkToClient::ptr new_client = TalkToClient::new_(service_ref_, this);
        acceptor_.async_accept(new_client->sock(), boost::bind(&TestServer::handleAccept, this, new_client,_1));
    }
    void TestServer::addClient(const TalkToClient::ptr& c){
        clients_.insert(c);
    }
    void TestServer::removeClient(const TalkToClient::ptr& client){
        clients_.erase(client);
    }
    void TestServer::processData(TalkToClient::ptr c, const std::string& m){
        if (recv_callback_){
            recv_callback_(c, m);
        }
    }
    size_t TestServer::isDataComplete(const std::string& data){
        bool found = check_completed_ ? check_completed_(data) : true;
        return found ? 0 : 1;
    }
    void TestServer::sendToAll(const std::string& m){
        for(auto& client: clients_){
            client->doWrite(m);
        }
    }
};