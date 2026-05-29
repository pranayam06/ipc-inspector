#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "ring_buffer.h"

int main() {
    int fd = shm_open("/ipc-channel", O_RDWR, 0666);
    size_t size = sizeof(RingBuffer);
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 ); 
    // note: not map fixed 
    RingBuffer* rbuf = (RingBuffer*) ptr; 

    Message out;
    uint32_t inspector_index = 0;
    uint32_t wr;
    
    while (true) {
        wr = rbuf->get_write();
        if (wr - inspector_index > 4) {
            inspector_index = wr - 4;
        }
        if (wr > inspector_index) { 
            uint32_t s1 = rbuf->get_seq(inspector_index % 4);
            if (s1 & 1) continue;
            out = rbuf->peek(inspector_index);
            uint32_t s2 = rbuf->get_seq(inspector_index % 4);
            if (s1 != s2) continue;
            inspector_index++;
            std::cout << out.data << " ts=" << out.timestamp_ns << "\n";
        }
    };
    sleep(10);
}