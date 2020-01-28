#include "random.h"
#include <cstdlib>
#include <ctime>
#include <vector>
namespace test_np{
    std::string randomString(size_t max_length, const std::string& chars){
        if (!max_length || chars.empty()){
            return "";
        }
        size_t length = rand() % max_length + 1; 

        std::vector<size_t> indexes_chars(length); 
        for (size_t i = 0; i < length; ++i) {
           indexes_chars.emplace_back(rand() % chars.length());
        }

        std::string res;
       for (auto i : indexes_chars){
           res.push_back(chars[i]);
       } 
       return res;
    }
}
