#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main() {
    int fd = shm_open("/ipc-channel", O_CREAT | O_RDWR, 0666);
    size_t size = 64;
    ftruncate(fd, size); 
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 ); 
    // note: not map fixed 
    char* buffer = (char*) ptr; 
    std::strcpy(buffer, "hello");
    sleep(10);
}