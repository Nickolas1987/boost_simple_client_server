#include "test_message_creator.h"
#include <sstream>
namespace test_np{
    std::string TestMessageCreator::create(const TestMsg& val) const {
        std::ostringstream os;
        {
          boost::archive::binary_oarchive oa(os);
          oa << val;
        }
        auto res = os.str();
        int  res_len = static_cast<int>(res.size());
        std::string res_len_str;
        res_len_str.resize(sizeof(int));
        memcpy(&res_len_str[0], &res_len, sizeof(int)); 
        return res_len_str + res;
    }
}