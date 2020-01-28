#pragma once
#include "test_client.h"
#include "protocol.h"
#include "timer.h"
#include <string>
#include "test_message_parser.h"
#include "test_message_creator.h"
namespace test_np{
    class ClientLogic{
      public:
        ClientLogic(io_service& service, const std::string& address, int port);
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
        deadline_timer           set_dl_;
        deadline_timer           get_dl_;
        mutable Timer            set_timer_;
        mutable Timer            get_timer_;
        char                     mes_separ_;
        TestMessageParser   parser_;
        TestMessageCreator  creator_;
    };
}