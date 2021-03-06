#pragma once
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/thread/shared_mutex.hpp>
#include "test_server.h"
#include "protocol.h"
#include "imessage_parser.h"
#include "imessage_creator.h"
#include "timer.h"
#include "iconnection_handler.h"
#include <string>
namespace test_np{
    class ServerLogic: public IConnectionHandler{
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
        ServerLogic(io_service& service, int port, const boost::shared_ptr<IMessageParser<TestMsg>>& parser, const boost::shared_ptr<IMessageCreator<const TestMsg&>>& creator);
      private:
        void        acceptClient(TalkToClient::ptr);
        void        recvMsg(TalkToClient::ptr, const std::string&);
        bool         checkMsg(const std::string&) const;
        void        printStat() const;
        TestServer          server_;
        Store               messages_;
        Statistics          statistics_;
        mutable Timer       timer_;
        boost::shared_ptr<IMessageParser<TestMsg>>   parser_;
        boost::shared_ptr<IMessageCreator<const TestMsg&>>  creator_;
    };
}