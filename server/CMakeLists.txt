cmake_minimum_required(VERSION 3.0) 
project(test_server)                    
                
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)


find_package(Boost 1.71.0 COMPONENTS filesystem system thread date_time program_options serialization REQUIRED)

include_directories(../protocol)
include_directories(../sha256)
include_directories(../timer)
set(SOURCE_EXE main.cpp server_logic.cpp test_server.cpp talk_to_client.cpp ../sha256/sha256.cpp ../protocol/test_message_parser.cpp ../protocol/test_message_creator.cpp)       
set(HEADER_EXE itest_server.h server_logic.h test_server.h talk_to_client.h)
add_executable(test_server ${SOURCE_EXE})

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(test_server Boost::filesystem Boost::system Boost::thread Boost::date_time Boost::program_options Boost::serialization)
endif()

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
  set(OPENSSL_USE_STATIC_LIBS TRUE)
  message("OPENSSL FOUND!")
endif()
target_link_libraries(test_server ssl crypto)





