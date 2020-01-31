#pragma once
#include "itest_client.h"
#include "iconnection_handler.h"
#include <boost/unordered_set.hpp>
#include <functional>
namespace test_np{
    class TestClient: public ITestClient{
      public:
        TestClient(io_service& service, IConnectionHandler& handler, const std::string& address, int port);
        ~TestClient();
        void   write(const std::string&);
        void stop();
        void processData(TalkToServer::ptr, const std::string&);
        size_t isDataComplete(const std::string&);
      private:
        io_service& service_ref_;
        IConnectionHandler& connection_handler_;
        TalkToServer::ptr  connection_;
    };
}