#include <cstdint>

namespace apex{

    static constexpr int64_t RING_BUFFER_SIZE = 1024; 
    static_assert((RING_BUFFER_SIZE & (RING_BUFFER_SIZE - 1)) == 0, "Ring buffer must be a power of 2");



}

    