#pragma once
#include "protocol.h"
#include <utility>
namespace test_np{
    class TestMessageParser{
      public:
        std::pair<int, TestMsg> parse(const std::string&) const;
        int getLength(const std::string&) const;
    };
}
