#include <cstdint>
#include <atomic>

class RingBuffer {
    private: 
       std::atomic<uint32_t>write; 
        std::atomic<uint32_t> read; 
        char buffer[64];
    public: 
        RingBuffer(); 
        void set_read(uint32_t new_read);
        void set_write(uint32_t new_write);
        uint32_t get_read();
        uint32_t get_write();
};

static_assert(std::atomic<uint32_t>::is_always_lock_free);
