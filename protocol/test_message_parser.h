#pragma once
#include "protocol.h"
#include "imessage_parser.h"
#include <utility>
namespace test_np{
    class TestMessageParser: public IMessageParser<TestMsg>{
      public:
        TestMsg parse(const std::string&) const;
        int getLength(const std::string&) const;
        bool isValid(const std::string&) const;
    };
}
