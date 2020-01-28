#pragma once
#include "itest_client.h"
#include <boost/unordered_set.hpp>
#include <functional>
namespace test_np{
    class TestClient: public ITestClient{
      public:
        TestClient(io_service& service, const std::string& address, int port,  
                    const std::function<void (TalkToServer::ptr, const std::string&)>& recv_callback = std::function<void (TalkToServer::ptr, const std::string&)>(),
                    const std::function<bool (const std::string&)>& check = std::function<bool (const std::string&)>());
        ~TestClient();
        void   write(const std::string&);
        void stop();
        void processData(TalkToServer::ptr, const std::string&);
        size_t isDataComplete(const std::string&);
      private:
        io_service& service_ref_;
        TalkToServer::ptr  connection_;
        std::function<void (TalkToServer::ptr, const std::string&)> recv_callback_;
        std::function<bool (const std::string&)> check_completed_;
    };
}