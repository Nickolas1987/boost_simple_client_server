#include "server_logic.h"
#include <sstream>
#include <openssl/sha.h>
#include <chrono>
#include <iostream>
#include "sha256.h"
#define  WAIT_INTERVAL 30000

namespace test_np{
    ServerLogic::ServerLogic(io_service& service, int port, char separ) : server_(service, port, boost::bind(&ServerLogic::acceptClient, this, _1), 
                                                                                                 boost::bind(&ServerLogic::recvMsg, this, _1, _2),
                                                                                                 boost::bind(&ServerLogic::checkMsg, this, _1)),
                                                          dl_(service), timer_(dl_, boost::bind(&ServerLogic::printStat, this)), mes_separ_(separ){
        timer_.reset(WAIT_INTERVAL);
    }
    void ServerLogic::acceptClient(TalkToClient::ptr client){
        /*
          Get client ip
        */
        boost::asio::ip::tcp::endpoint remote_ep = client->sock().remote_endpoint();
        boost::asio::ip::address remote_ad = remote_ep.address();
        auto time_now_sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch());
        /*
          Create new client id
        */
        std::string id = remote_ad.to_string() + std::to_string(remote_ep.port()) + std::to_string(time_now_sec.count());

        client->doWrite(creator_.create({SET_ID_MSG, sha256(id)}));
    }
    void ServerLogic::printStat() const {
        auto& stat_list = statistics_.get<0>();
        for (auto stat_iter: stat_list){
            std::cerr << "Client ID: " <<stat_iter.id_client_ << " In messages: " << stat_iter.in_count_ << " Out messages: " << stat_iter.out_count_ << '\n';
        }
        timer_.reset(WAIT_INTERVAL);
    }
    bool ServerLogic::checkMsg(const std::string& msg) const{
        return parser_.getLength(msg) >= 0;
    }
    void ServerLogic::recvMsg(TalkToClient::ptr client, const std::string& msg){
        try{
          auto parse_res = parser_.parse(msg); 
          TestMsg new_msg = parse_res.second;
          /*
              Check is client id is not empty
          */
          if (!new_msg.clients_id_list_.empty()){
              switch(new_msg.msg_type_){
                  case GET_MSG:
                  {
                    auto& list = messages_.get<0>();
                    auto  fnd_iter = list.find(new_msg.msg_id_);      

                    if (fnd_iter != list.end()){
                      client->doWrite(creator_.create({RESPONSE_MSG, new_msg.msg_id_, fnd_iter->data_}));
                      messages_.erase(fnd_iter);      

                      /*
                        Change statistics
                      */
                      auto& stat_list = statistics_.get<0>();  
                      auto stat_iter = stat_list.find(new_msg.clients_id_list_.front());
                      if (stat_iter != stat_list.end()){
                          ++(stat_iter->out_count_);
                      }
                    }
                    else{
                      client->doWrite(creator_.create({NO_DATA_MSG, new_msg.msg_id_}));
                    }
                  }
                  break;
                  case PUT_MSG:
                  {
                    /*
                      Add new record in storage
                    */
                    TestStoredData new_data{ new_msg.msg_id_, new_msg.msg_data_};
                    messages_.insert(new_data);
                    /* 
                      Find client record in statistics
                    */
                    auto& stat_list = statistics_.get<0>();
                    auto stat_iter = stat_list.find(new_msg.clients_id_list_.front());
                    if (stat_iter != stat_list.end()){
                        ++(stat_iter->in_count_);
                    }
                    else {
                    /* 
                      Add record to statistics
                    */      

                        TestStatistics new_statistics{new_msg.clients_id_list_.front(), 1, 0};
                        statistics_.insert(new_statistics);
                    }
                    /*
                      Send new list of id to all clients 
                    */
                    auto& list = messages_.get<0>();
                    std::vector<std::string> clients_id_list_;
                    for (auto& iter : list){
                      clients_id_list_.emplace_back(iter.id_);
                    }
                    server_.sendToAll(creator_.create({LIST_ID_MSG, new_msg.msg_id_, new_msg.msg_data_, clients_id_list_ }));
                  }
                  break;
              }
          }
        }
        catch(boost::archive::archive_exception& e){
          std::cerr << "Exception " << e.what() << " on message " << msg << '\n';
        }
        client->doRead();
    }
};