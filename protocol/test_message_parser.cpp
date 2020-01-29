#include "test_message_parser.h"
#include <cstring>
#include <sstream>

namespace test_np{
    TestMsg TestMessageParser::parse(const std::string& msg) const{
        TestMsg new_msg;
        int len = getLength(msg);
        if (len >= 0){
          {
            std::istringstream is(std::string(msg.begin() + sizeof(int), msg.begin() + sizeof(int) + len));
            boost::archive::binary_iarchive ia(is);
            ia >> new_msg;
          }
        }
        else{
          new_msg.msg_type_ = UNDEFINED;
        }
        return new_msg;
    }
    int TestMessageParser::getLength(const std::string& val) const{
        if (val.size() >= sizeof(int)){
          auto len_str = std::string(val.begin(), val.begin() + sizeof(int));
          int len = 0; 
          memcpy( &len, &len_str[0],  sizeof(int));

          if (len <= val.size() - len_str.length()){
              return len;
          }
        }
        return -1;
    }
    bool TestMessageParser::isValid(const std::string& val) const{
        return getLength(val) >= 0;
    }
}