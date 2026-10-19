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

