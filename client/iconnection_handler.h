#pragma once
#include "talk_to_server.h"
namespace test_np{
    class IConnectionHandler{
      public:
        virtual void recvMsg(TalkToServer::ptr, const std::string&) = 0;
        virtual bool checkMsg(const std::string&) const = 0;
    };
}
