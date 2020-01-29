#pragma once
#include "protocol.h"
namespace test_np{
    template<class T>
    class IMessageCreator{
      public:
        virtual std::string create(T) const = 0;
    };
}
