#pragma once
#include "protocol.h"
#include "imessage_creator.h"
namespace test_np{
    class TestMessageCreator: public IMessageCreator<const TestMsg&>{
      public:
        std::string create(const TestMsg&) const;
    };
}
