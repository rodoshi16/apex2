#include <cstdint>
#include "apex/order.hpp"
#include "apex/event.hpp"
#include <list>
#include <unordered_map>
#include <vector>

namespace apex{


    class PriceLevel{
    public:

        void add_order(Order order);
        bool cancel_order(uint64_t order_id);
        // take quantity of amount of shares and the event to record: trade/rest
        uint64_t fill(uint64_t quantity, std::vector<Event>& events_out);
        

        int64_t price() const {
            return price_;
        }

        uint64_t total_quantity() const {
            return total_quantity_;
        }

        bool empty() const {
            return orders_.empty();
        }



    private:
        int64_t price_;
        uint64_t total_quantity_ = 0; 


        //doubly linked list called orders
        std::list<Order> orders_;
        //hashmap mapping the order id to the Node in LL
        std::unordered_map<uint64_t, std::list<Order>::iterator> order_map_;
    };
}