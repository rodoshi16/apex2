***Inside the disruptor class***

Ring buffer has 1024 pre allocated slots. 

int - 32 bits
int64_t = 64 bits

constexpr - constant var 
static - only to be used in this file 

Declaration for size:

static constexpr it64_t RING_BUFFER_SIZE = 1024

static_assert: checks a condition when your code is compiled - we need to make sure that the ring buffer size is a power of 2. 

Even though we declare the ring buffer to be 1024, the developer can edit it anytime. We therefore must always check.

size & (size-1) == 0 - means it is a power of 2


***Producer and consumer model***

- Regular var can be changed or read by multiple threads, but if they touch it at the same time, you have a race condition. 

An atomic variable prevents that race on the var itself. You can make the counters atomic because the producer and consumer need to communicate safely about your progress. Atomic makes the individual reads and writes safe but it doesnt make the whole ring buffer magically safe on its own. 

std::atomic makes sure we can safely access the shared counter

** Why use alignas(64)**

- If two diff var are in the same cache, two threads accidently fight over then same cache line and can cause false sharing. We want to make sure that variables are kept in different cache lines. 

Alignas(64) means place the object on a 64 byte memory boundary. This prevents false sharing and is trying to reduce cache contention. It's a performance hint to keep things from stepping on each other in memory. 


*** How to update these atomic variables?***

.store() is the function you can use to write a value to the atomic variable. In this case, they both start at -1, relaxed is used because at initialization you dont need to establish ordering with other memory operations. 


*** How to update the variables?***

Take the load function which read the current value and then add 1 to get to the next slot. 

producer_sequence_.load(std::memory_order_release) + 1 


*** What is the algo for producer taking an order and consumer consuming one?***

1. calc next slot = current_producer number + 1 
2. wrap it by subtracting 1024 (since thats the cap)
3. check if c < wrap -> this means consumer hasn't consumed so far so you would be overriding values 

ex: if consumer is at 2 and your wrap = 3, 2 < 3 - return false here

4. if not, then calculate the index, update its order and sequence
5. update the producer var now and return TRUE 


Same for consumer except check if p < c -> that would mean there is nothing to read yet since no orders published that far so return false

otherwise update the var 

*** memory_order_release vs relaxed vs acquire*** 

use release when you want to announce the change to the consumer/producer

relaxed is when you just want to know the counter value, reading the var

acquire - use when reading a published value and you need to synchronize with the release operation. git





