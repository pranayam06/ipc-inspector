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

    char out[16];

    
    while (true) {
        if (rbuf->consume(out)) {
            std::cout << out << "\n";
        }
    };
    sleep(10);
}