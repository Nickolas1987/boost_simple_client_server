#pragma once
#include "talk_to_client.h"
namespace test_np{
    class ITestServer{
       public:
         virtual void handleAccept(TalkToClient::ptr client, const boost::system::error_code & err) = 0;
         virtual void addClient(const TalkToClient::ptr& client) = 0;
         virtual void removeClient(const TalkToClient::ptr& client) = 0;
         virtual void processData(TalkToClient::ptr, const std::string&) = 0;
         virtual size_t isDataComplete(const std::string&) = 0;
         virtual void sendToAll(const std::string&) = 0;
         virtual ~ITestServer(){}
    };
}