#pragma once
#include <cstdint>
# include "apex/order.hpp"

namespace apex{

enum class EventType: uint8_t{
    OrderAdded,
    OrderCancelled, 
    OrderModified, 
    OrderPartiallyFilled, 
    OrderFilled, 
    TradeExecuted
};


struct Event {
    //type, timestamp, orderid, trade_id, price, quantity, side
    EventType type; 
    uint64_t timestamp_ns; 
    uint64_t order_id; 
    uint64_t trade_id; 
    int64_t price; 
    uint64_t quantity; 
    Side side; 

};





}