#pragma once
#include "test_client.h"
#include "protocol.h"
#include "timer.h"
#include <string>
#include "imessage_parser.h"
#include "imessage_creator.h"
#include "iconnection_handler.h"
namespace test_np{
    class ClientLogic: public IConnectionHandler{
      public:
        ClientLogic(io_service& service, const std::string& address, int port, const boost::shared_ptr<IMessageParser<TestMsg>>& parser, 
                                                                     const boost::shared_ptr<IMessageCreator<const TestMsg&>>& creator);
        ~ClientLogic();
      private:
        void        recvMsg(TalkToServer::ptr, const std::string&);
        void        setMsg();
        void        getMsg();
        bool        checkMsg(const std::string&) const;
        TestClient               client_;
        std::string              address_;
        int                      port_;
        std::string              id_;
        std::vector<std::string> id_mes_list_;
        mutable Timer            set_timer_;
        mutable Timer            get_timer_;
        boost::shared_ptr<IMessageParser<TestMsg>>   parser_;
        boost::shared_ptr<IMessageCreator<const TestMsg&>>  creator_;
    };
}