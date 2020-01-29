#pragma once
namespace test_np{
    template<class T>
    class IMessageParser{
      public:
        virtual T parse(const std::string&) const = 0;
        virtual bool isValid(const std::string&) const = 0;
    };
}
