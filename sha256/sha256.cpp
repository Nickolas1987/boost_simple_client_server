#include "sha256.h"
#include <sstream>
namespace test_np{
    std::string sha256(const std::string& str){
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);
        std::stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
            ss << (int)hash[i];
        }
        return ss.str();
    }
}
