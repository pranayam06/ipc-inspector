#include <sys/mman.h> 

int main() {
    shm_unlink("/ipc-channel");
}