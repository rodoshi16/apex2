***Apex - a low latency exchange simulator in C++*** 

**How do you squeeze performance out of hardware?**

I thought about performance at three specific hardware levels 

1. Cache
2. Memory
3. Instructions

**Cache Level** - A CPU cache line is 64 bytes. If two threads write to variables that share a cache line, they constantly invalidate each other's cache even though they're touching different variables. That is false sharing. 

In the disruptor I have two atomic sequence numbers - producer and consumer. Since they're 8 bytes each and might end on the same cache line, i use alignas(64) to force each onto its own cache line and prevent false sharing. 

In the ring buffer slot - each contains an order struct. If adjacent slots share cache lines, two producers writing to adj slots would cause false sharing. The Slot struct also uses alignas(64) so each slot occupies its own cache line. 

**Memory level** - For heap allocation, every call to new goes to the OS allocator - it finds free memory, marks it used and acquires an internal lock. This is slow and unpredictable. At 5M orders per second, extremely slow. 

Ring buffer allocates 1024 Order slots at the start - no allocation during trading. This is also memory efficient since the same slots get reused multiple times without having a list that will extend forever. The object pool allocates 65,536 Order objects. 

**CPU instruction level**

To find the ring buffer index, the naive approach is N % 1024. Modulo requires int divison which 20-40 CPU cycles on modern hardware. 

By making the buffer size a power of 2, I can replace the mod with a bitmask: N & (BUFFER_SIZE - 1). Identical result but one CPU cycle. 

I also chose acquire/release memory ordering which compiles to ordinary load and stores with zero overhead, instead of using the default seq_cst. 




References:

- LMAX Disruptor: https://lmax-exchange.github.io/disruptor/ 
- Orstein uhlenbeck process for simulation: https://www.quantt.co.uk/resources/ornstein-uhlenbeck-process 
