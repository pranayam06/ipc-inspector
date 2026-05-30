#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#ifdef USE_MUTEX
#include "ring_buffer_mutex.h"
#else
#include "ring_buffer.h"
#endif
#include <time.h>
#include <vector>
#include <algorithm>

int main() {
    int fd = shm_open("/ipc-channel", O_RDWR, 0666);
    size_t size = sizeof(RingBuffer);
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 ); 
    // note: not map fixed 
    RingBuffer* rbuf = (RingBuffer*) ptr;  
    std::vector<uint64_t> stalls;
    Message out;
    timespec ts;
    int spins = 0;

    rbuf->consumer_ready.store(true, std::memory_order_release);
    clock_gettime(CLOCK_MONOTONIC, &ts); 
    uint64_t now = ts.tv_sec * 1000000000ULL + ts.tv_nsec;
    std::cout << "start consume" << now << "\n";



    
    while (true) {
        if (rbuf->consume(&out)) {
            clock_gettime(CLOCK_MONOTONIC, &ts); 
            uint64_t now = ts.tv_sec * 1000000000ULL + ts.tv_nsec;
            uint64_t latency = now - out.timestamp_ns;
            // std::cout << now << "\n";
            stalls.push_back(latency);
            //std::cout << out.data << " ts=" << out.timestamp_ns << "\n";  
            spins = 0;
        }
        else{
            spins++;
            if (spins > 100000000) break;
        }
    };

    std::sort(stalls.begin(), stalls.end()); 
    std::cout << "p50= " << stalls[stalls.size() / 2] << "\n"; 
    std::cout << "p90= " << stalls[stalls.size() * 9 / 10] << "\n"; 
    std::cout << "p99= " << stalls[stalls.size() * 99 / 100] << "\n";

    sleep(3);
}