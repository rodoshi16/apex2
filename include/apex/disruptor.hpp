#include <cstdint>
#include "apex/order.hpp"
#include <atomic>
#include <array>

namespace apex{

    //disruptor has 1024 slots and assert to check if its a power of 2
    static constexpr int64_t RING_BUFFER_SIZE = 1024; 
    static_assert((RING_BUFFER_SIZE & (RING_BUFFER_SIZE - 1)) == 0, "Ring buffer must be a power of 2");


    //each slot will have order obj and a sequence to know the number the order is in when it arrived
    // this is cuz the slots wrap around so will loose its order in the sequence otherwise
    struct Slot{
        Order order; 
        uint64_t Sequence; 
    };

    class Disruptor{

        public:
            Disruptor(){
                //store writes -1 to the variables
                // memory order relaxed because we dont need sync rn
                producer_sequence_.store(-1, std::memory_order_release);
                consumer_sequence_.store(-1, std::memory_order_release);
            }
        
        bool publish(const Order& order){
            int64_t next = producer_sequence_.load(std::memory_order_relaxed) + 1;

            int64_t wrap_point = next - RING_BUFFER_SIZE;
            if (consumer_sequence_.load(std::memory_order_acquire) < wrap_point){
                return false; 
            }

            int64_t index = next & (RING_BUFFER_SIZE - 1);
            buffer_[index].order = order;
            buffer_[index].Sequence = next; 

            producer_sequence_.store(next, std::memory_order_release);
            return true; 
        }

        bool consume(Order& order_out){

            int64_t next = consumer_sequence_.load(std::memory_order_release) + 1;
            
            if (producer_sequence_.load(std::memory_order_acquire) < next) {
                return false;
            }

            int64_t index = next & (RING_BUFFER_SIZE -1);
            order_out = buffer_[index].order;


            consumer_sequence_.store(next, std::memory_order_release);
            return true;

        }

        int64_t producer_sequence() const{
            return producer_sequence_.load(std::memory_order_relaxed);
        }

        int64_t consumer_sequence() const{
            return consumer_sequence_.load(std::memory_order_relaxed);

        }

        private:
            //producer and consumer are diff atomic variables on diff cache lines
            alignas(64) std::atomic<int64_t>producer_sequence_;
            alignas(64) std::atomic<int64_t>consumer_sequence_;
            alignas(64) std::array<Slot, RING_BUFFER_SIZE> buffer_; 
    };


}

    