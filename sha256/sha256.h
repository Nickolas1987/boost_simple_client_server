#pragma once
#include <string>
#include <openssl/sha.h>
namespace test_np{
    std::string sha256(const std::string& str);
}
