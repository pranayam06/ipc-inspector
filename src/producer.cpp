#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>
#ifdef USE_MUTEX
    #include "ring_buffer_mutex.h"
#else
    #include "ring_buffer.h"
#endif

int main() {
    int fd = shm_open("/ipc-channel", O_CREAT | O_RDWR, 0666);
    size_t size = sizeof(RingBuffer);
    ftruncate(fd, size);  
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    RingBuffer* rbuf = new (ptr) RingBuffer();

    while (!rbuf->consumer_ready.load(std::memory_order_acquire)) {}

    for (int i = 0; i< 100000000; i++) {
        std::string msg = "msg" + std::to_string(i);
        rbuf->publish(msg.c_str());
    }
    sleep(5);
    shm_unlink("/ipc-channel"); 
}