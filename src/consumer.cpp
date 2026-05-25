#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int main() {
    int fd = shm_open("/ipc-channel", O_RDONLY, 0666);
    size_t size = 64;
    void* ptr = mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0 ); 
    // note: not map fixed 
    char* buffer = (char*) ptr; 
    while(true) {
        std::cout << buffer;
        sleep(1);
    }
    sleep(10);
}