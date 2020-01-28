#pragma once
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/thread/shared_mutex.hpp>
#include "test_server.h"
#include "protocol.h"
#include "test_message_parser.h"
#include "test_message_creator.h"
#include "timer.h"
#include <string>
namespace test_np{
    class ServerLogic{
      public:
        typedef boost::multi_index_container<TestStoredData,
                    boost::multi_index::indexed_by<
                                    boost::multi_index::ordered_unique<
                                      boost::multi_index::member<TestStoredData, std::string, &TestStoredData::id_>
                                    >,
                                    boost::multi_index::ordered_non_unique<
                                      boost::multi_index::member<TestStoredData, std::string, &TestStoredData::data_>
                                    >
                    >
        > Store;
        typedef boost::multi_index_container<TestStatistics,
                    boost::multi_index::indexed_by<
                                    boost::multi_index::ordered_unique<
                                      boost::multi_index::member<TestStatistics, std::string, &TestStatistics::id_client_>
                                    >,
                                    boost::multi_index::ordered_unique<
                                      boost::multi_index::member<TestStatistics, size_t, &TestStatistics::in_count_>
                                    >,
                                    boost::multi_index::ordered_non_unique<
                                      boost::multi_index::member<TestStatistics, size_t, &TestStatistics::out_count_>
                                    >
                    >
        > Statistics;
        ServerLogic(io_service& service, int port, char mes_separ = '\n');
      private:
        void        acceptClient(TalkToClient::ptr);
        void        recvMsg(TalkToClient::ptr, const std::string&);
        bool         checkMsg(const std::string&) const;
        void        printStat() const;
        TestServer          server_;
        Store               messages_;
        Statistics          statistics_;
        deadline_timer      dl_;
        mutable Timer       timer_;
        char                mes_separ_;
        TestMessageParser   parser_;
        TestMessageCreator  creator_;
    };
}