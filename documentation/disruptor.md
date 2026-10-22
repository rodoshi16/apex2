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



