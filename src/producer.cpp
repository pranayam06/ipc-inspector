#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>

int main() {
    int fd = shm_open("/ipc-channel", O_CREAT | O_RDWR, 0666);
    size_t size = 64;
    ftruncate(fd, size); 
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 ); 
    // note: not map fixed 
    char* buffer = (char*) ptr; 
    int ctr = 0;
    while (true) {
            ctr += 1;
            std::strcpy(buffer, ("message" + std::to_string(ctr)).c_str());
            sleep(1);
    }
    sleep(10);
}