#include <cstdint>
#include <atomic>
#include <time.h>

struct Message {
    uint64_t timestamp_ns; 
    char data[8]; 
};

class RingBuffer {
    private: 
        pthread_mutex_t mutex;
        uint32_t write; 
        uint32_t read; 
        Message buffer[1000];
    public: 
        RingBuffer(); 
        uint32_t get_write();
        void increment_seq(size_t);
        bool check_seq(size_t);
        uint32_t get_seq(size_t);
        void publish(const char*);
        bool consume(Message*);
        Message peek(uint32_t); 
};

