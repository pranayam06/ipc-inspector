#include <cstdint>
#include <atomic>
#include <time.h>

struct Message {
    uint64_t timestamp_ns; 
    char data[8]; 
};

class RingBuffer {
    private: 
        std::atomic<uint32_t>write; 
        std::atomic<uint32_t> read; 
        Message buffer[4];
        std::atomic<uint32_t> seq[4];
    public: 
        RingBuffer(); 
        void set_read(uint32_t new_read);
        void set_write(uint32_t new_write);
        uint32_t get_read();
        uint32_t get_write();
        void increment_seq(size_t);
        bool check_seq(size_t);
        uint32_t get_seq(size_t);
        void publish(const char*);
        bool consume(Message*);
        Message peek(uint32_t); 

};

static_assert(std::atomic<uint32_t>::is_always_lock_free);
