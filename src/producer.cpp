#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include "ring_buffer.h"

int main() {
    int fd = shm_open("/ipc-channel", O_CREAT | O_RDWR, 0666);
    size_t size = sizeof(RingBuffer);
    ftruncate(fd, size);  
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
     

    RingBuffer* rbuf = new (ptr) RingBuffer();
    
    for (int i = 0; i< 8; i++) {
        std::string msg = "message" + std::to_string(i);
        rbuf->publish(msg.c_str());
        sleep(1);

    }
    sleep(10);
    shm_unlink("/ipc-channel");
}