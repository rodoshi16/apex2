#pragma once
#include <map>
#include <unordered_map>
#include "apex/order.hpp"

namespace apex{

class OrderBook{

    struct OrderLocation{
        Side side;
        int64_t price;


    };
    
    std::unordered_map<uint64_t, OrderLocation> order_location;





}




}; 