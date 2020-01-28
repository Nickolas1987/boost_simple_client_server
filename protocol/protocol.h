#pragma once
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <string>
#include <vector>
namespace test_np{
    enum MSG_TYPE {SET_ID_MSG, GET_MSG, PUT_MSG, LIST_ID_MSG, RESPONSE_MSG, NO_DATA_MSG};
    struct TestStoredData{
        std::string                                   id_;
        std::string                                 data_;
    };
    struct TestStatistics{
        std::string                            id_client_;
        mutable size_t                          in_count_;
        mutable size_t                         out_count_;
    };
    struct TestMsg{       
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version){
          ar & msg_type_;
          ar & msg_id_;
          ar & msg_data_;
          ar & clients_id_list_;
        }
        MSG_TYPE                                msg_type_;
        std::string                               msg_id_;
        std::string                             msg_data_;
        std::vector<std::string>         clients_id_list_;
    };
}
