#pragma once
#include "protocol.h"
namespace test_np{
    class TestMessageCreator{
      public:
        std::string create(const TestMsg&) const;
    };
}
