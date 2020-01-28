cmake_minimum_required(VERSION 3.0)
project(test_client)
                                        
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)


find_package(Boost 1.71.0 COMPONENTS filesystem system thread date_time program_options serialization REQUIRED)

include_directories(../protocol)
include_directories(../sha256)
include_directories(../random)
include_directories(../timer)
set(SOURCE_EXE main.cpp client_logic.cpp test_client.cpp talk_to_server.cpp ../sha256/sha256.cpp ../random/random.cpp ../protocol/test_message_parser.cpp ../protocol/test_message_creator.cpp)  
set(HEADER_EXE itest_client.h client_logic.h test_client.h talk_to_server.h)
add_executable(test_client ${SOURCE_EXE})

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(test_client Boost::filesystem Boost::system Boost::thread Boost::date_time Boost::program_options Boost::serialization)
endif()

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
  set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()
target_link_libraries(test_client ssl crypto)





