#include "client_logic.h"
#include <boost/program_options.hpp>
#include <iostream>
using namespace boost::asio;
namespace po = boost::program_options;

io_service service;

int main(int argc, char** argv){
    po::options_description desc("Description");
    desc.add_options()
    ("help,h", "help me")
    ("address,a", po::value<std::string>(), "address to connect")
    ("port,p", po::value<int>(), "port to connect");
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
    if(vm.count("address") && vm.count("port")){
      test_np::ClientLogic new_server(service, vm["address"].as<std::string>(), vm["port"].as<int>());
      service.run();
    }
    return 0;
}

