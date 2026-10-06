#pragma once
#include <cstdint>

namespace apex{

enum class Side{
    Bid,
    Ask
};

enum class OrderType{
    Market,
    Limit, 
    FOK,
    IOC
};

enum class OrderStatus{
    New,
    PartiallyFilled,
    Filled,
    Cancelled,
    Rejected

};




struct Order{
    uint64_t order_id;
    uint64_t timestamp_ns;
    uint64_t price; 
    uint64_t quantity;
    uint64_t filled_Quantity;
    Side side;
    OrderType type;
    OrderStatus status;


    uint64_t remaining_quantity() const {
        return quantity - filled_Quantity;
    }

    bool is_buy() const {
        return side == Side::Bid;
    }

    bool is_sell()  const {
        return side == Side::Ask;
    }


}; 


}