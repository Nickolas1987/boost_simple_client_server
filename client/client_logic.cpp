#include "client_logic.h"
#include <sstream>
#include <openssl/sha.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "sha256.h"
#include "random.h"
namespace test_np{
    static const int LOW_BOUND_TIME = 250;
    static const int UP_BOUND_TIME = 500;
    static const int SET_TIME = 5000;
    static const int MAX_LEN_MES   = 30;
    ClientLogic::ClientLogic(io_service& service, const std::string& address, int port, const boost::shared_ptr<IMessageParser<TestMsg>>& parser, 
                                                                               const boost::shared_ptr<IMessageCreator<const TestMsg&>>& creator) : 
                                                                                        client_(service, *this, address, port), 
                                                                                        address_(address), port_(port),
                                                                                        set_timer_(service, boost::bind(&ClientLogic::setMsg, this)),
                                                                                        get_timer_(service, boost::bind(&ClientLogic::getMsg, this)),
                                                                                        parser_(parser), creator_(creator){
        if (!parser_ || !creator_){
          throw std::runtime_error("bad constructor params");
        }
        std::srand(unsigned(std::time(0)));
    }
    ClientLogic::~ClientLogic(){
        set_timer_.cancel();
        get_timer_.cancel();
        client_.stop();
    }
    void ClientLogic::getMsg(){
        size_t index = rand() % id_mes_list_.size();
        client_.write(creator_->create({GET_MSG, id_mes_list_.at(index), "", {id_}}));
    }
    void ClientLogic::setMsg(){
        std::string new_msg_data = randomString(MAX_LEN_MES);
        auto time_now_sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch());
        /*
          Create new msg id
        */
        std::string id = address_ + std::to_string(port_) + std::to_string(time_now_sec.count()) + new_msg_data;
        client_.write(creator_->create({PUT_MSG, sha256(id), new_msg_data, {id_}}));
        set_timer_.reset(SET_TIME);
    }
    /*
      Callback to check is msg fully received
    */
    bool ClientLogic::checkMsg(const std::string& msg) const{
        return parser_->isValid(msg);
    }
    void ClientLogic::recvMsg(TalkToServer::ptr client, const std::string& msg){

        TestMsg new_msg = parser_->parse(msg);

        switch(new_msg.msg_type_){
            case SET_ID_MSG:
            {
              id_ = new_msg.msg_id_;
              set_timer_.reset(SET_TIME);
            }
            break;
            case RESPONSE_MSG:
            {
              std::cerr << "Message data received: " << new_msg.msg_data_ << '\n';
            }
            break;
            case NO_DATA_MSG:
            {
              std::cerr << "No data for id: " << new_msg.msg_id_ << '\n';
            }
            break;
            case LIST_ID_MSG:
            {
              id_mes_list_ =  new_msg.clients_id_list_;
              if (!id_mes_list_.empty()){
                get_timer_.reset(1000 + (LOW_BOUND_TIME + rand()%UP_BOUND_TIME));
              }
            }
            break;
        }

        client->doRead();
    }
};