#pragma once
#include "talk_to_client.h"
namespace test_np{
    class IConnectionHandler{
      public:
        virtual void acceptClient(TalkToClient::ptr) = 0;
        virtual void recvMsg(TalkToClient::ptr, const std::string&) = 0;
        virtual bool checkMsg(const std::string&) const = 0;
    };
}
