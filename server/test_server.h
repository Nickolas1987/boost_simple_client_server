#pragma once
#include "itest_server.h"
#include <boost/unordered_set.hpp>
#include <functional>
#include "iconnection_handler.h"
namespace test_np{
    class TestServer: public ITestServer{
      public:
        TestServer(io_service& service, int port, IConnectionHandler& handler);
        ~TestServer();
        void handleAccept(TalkToClient::ptr client, const boost::system::error_code & err);
        void addClient(const TalkToClient::ptr& client);
        void removeClient(const TalkToClient::ptr& client);
        void processData(TalkToClient::ptr, const std::string&);
        size_t isDataComplete(const std::string&);
        void sendToAll(const std::string&);
      private:
        io_service& service_ref_;
        ip::tcp::acceptor acceptor_;
        boost::unordered_set<boost::shared_ptr<TalkToClient> > clients_;
        IConnectionHandler& connection_handler_;
    };
}