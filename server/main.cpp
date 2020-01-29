#include "server_logic.h"
#include <boost/program_options.hpp>
#include "protocol.h"
#include "test_message_parser.h"
#include "test_message_creator.h"
#include <iostream>
using namespace boost::asio;
using namespace test_np;
namespace po = boost::program_options;

io_service service;

int main(int argc, char** argv){
    po::options_description desc("Description");
    desc.add_options()
    ("help,h", "help me")
    ("port,p", po::value<int>(), "listening port");
    po::variables_map vm;
    try{
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch(po::error& e){
        std::cout << e.what() << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }

    if(vm.count("help")){
        std::cout << desc << std::endl;
        return 1;
    }
    if(vm.count("port")){
      try{
        test_np::ServerLogic new_server(service, vm["port"].as<int>(), boost::shared_ptr<IMessageParser<TestMsg>>(new TestMessageParser), 
                                                             boost::shared_ptr<IMessageCreator<const TestMsg&>>(new TestMessageCreator));
        service.run();
      }
      catch(std::runtime_error& e){
        std::cout << e.what() << std::endl;
      }
    }
    return 0;
}

