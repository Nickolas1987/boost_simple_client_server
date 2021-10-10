#include "test_server.h"
namespace test_np{
    TestServer::TestServer(io_service& service, int port, IConnectionHandler& handler) : service_ref_(service), 
                                                                                         acceptor_(service, ip::tcp::endpoint(ip::tcp::v4(), port)),
                                                                                         connection_handler_(handler){
        auto start_client_ = TalkToClient::new_(service_ref_, *this);
        addClient(start_client_);
        acceptor_.async_accept(start_client_->sock(), boost::bind(&TestServer::handleAccept, this, start_client_,_1));
    }
    TestServer::~TestServer(){
    }
    void TestServer::handleAccept(TalkToClient::ptr client, const boost::system::error_code & err){
        client->start();
        connection_handler_.acceptClient(client);
        TalkToClient::ptr new_client = TalkToClient::new_(service_ref_, *this);
        addClient(new_client);
        acceptor_.async_accept(new_client->sock(), boost::bind(&TestServer::handleAccept, this, new_client,_1));
    }
    void TestServer::addClient(const TalkToClient::ptr& c){
        clients_.insert(c);
    }
    void TestServer::removeClient(const TalkToClient::ptr& client){
        clients_.erase(client);
    }
    void TestServer::processData(TalkToClient::ptr c, const std::string& m){
        connection_handler_.recvMsg(c, m);
    }
    size_t TestServer::isDataComplete(const std::string& data){
        bool found = connection_handler_.checkMsg(data);
        return found ? 0 : 1;
    }
    void TestServer::sendToAll(const std::string& m){
        for(auto& client: clients_){
            client->doWrite(m);
        }
    }
};