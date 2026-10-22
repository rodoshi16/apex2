#include "apex/price_level.hpp"

namespace apex{

    void PriceLevel::add_order(Order order){
        // add to the quantity, append to my linked list
        // map order id to Node
        total_quantity_ += order.remaining_quantity();
        orders_.push_back(order);
        //std::prev is the last order you inserted 
        order_map_[order.order_id] = std::prev(orders_.end());
    }

    bool PriceLevel::cancel_order(uint64_t order_id){
        // find hashmap[id]
        // if nothing exist - it will return order_map_end
        auto node = order_map_.find(order_id);
        if (node == order_map_.end()){
            return false; 
        }

        //subtract this orders quant from total quant
        total_quantity_ -= node->second->remaining_quantity();
        //remove the node from the doubly linked list
        orders_.erase(node->second);
        //remove order pair from hashmap
        order_map_.erase(node);
        return true;
    }

    uint64_t PriceLevel::fill(uint64_t quantity, std::vector<Event>& events_out){
        //track how much needs to be filled
        uint64_t remaining = quantity;

        while (!orders_.empty() && remaining > 0){
            //because of price time priority - gets the oldest order which is at the front
            Order& front = orders_.front();
            //calc the remaining
            uint64_t fill_qty = std::min(remaining, front.remaining_quantity());

            //filled quant increases, but total and remaining decreases
            front.filled_Quantity += fill_qty;
            total_quantity_ -= fill_qty;
            remaining -= fill_qty;

            //determine is it a filled order, partialy filled
            EventType etype = front.remaining_quantity() == 0
                ? EventType::OrderFilled
                : EventType::OrderPartiallyFilled;
            
            //record the event
            events_out.push_back(Event{
                etype, 
                0, 
                front.order_id, 
                0, 
                price_, 
                fill_qty,
                front.side
            });


            if (front.remaining_quantity() == 0){
                order_map_.erase(front.order_id);
                orders_.pop_front();
            }
        }

        return quantity - remaining;
    }
}