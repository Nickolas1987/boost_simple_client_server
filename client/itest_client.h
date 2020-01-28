#pragma once
#include "talk_to_server.h"
namespace test_np{
    class ITestClient{
       public:
         virtual void processData(TalkToServer::ptr, const std::string&) = 0;
         virtual size_t isDataComplete(const std::string&) = 0;
         virtual void   write(const std::string&) = 0;
         virtual void   stop() = 0;
         virtual ~ITestClient(){}
    };
}