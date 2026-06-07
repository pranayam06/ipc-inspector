#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "ring_buffer.h"
#include <sys/ioctl.h>
#include <vector>



int main(int argc, char* argv[]) {
    std::string filter = "";
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--filter" && i + 1 < argc) {
            filter = argv[i + 1];
            i++;
        }
    }
    int fd = shm_open("/ipc-channel", O_RDWR, 0666);
    size_t size = sizeof(RingBuffer);
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 ); 
    // note: not map fixed 
    RingBuffer* rbuf = (RingBuffer*) ptr; 

    uint64_t count = 0;
    uint64_t dropped = 0;
    uint64_t msgs_this_second = 0;
    uint64_t msgs_per_sec = 0;

    std::vector<uint64_t> latencies;
    timespec ld_ts, now_ts;
    uint64_t ld = 0; 
    clock_gettime(CLOCK_MONOTONIC, &ld_ts);

    Message out;
    uint32_t inspector_index = 0;
    uint32_t wr;
    
    while (true) {
        wr = rbuf->get_write();
        if (wr - inspector_index > 1000) {
            dropped += (wr-inspector_index - 1000);
            // no race conditions because if write progresses, the dropped messages counted in the next loop!
            inspector_index = wr - 1000;
        }
        if (wr > inspector_index) { 
            uint32_t s1 = rbuf->get_seq(inspector_index % 1000);
            if (s1 & 1) continue;
            out = rbuf->peek(inspector_index);
            uint32_t s2 = rbuf->get_seq(inspector_index % 1000);
            if (s1 != s2) continue;
            inspector_index++;
            count++;
            msgs_this_second += 1;
            // calculate latency from message timestamp
            timespec now_ts;
            clock_gettime(CLOCK_MONOTONIC, &now_ts);
            uint64_t now = now_ts.tv_sec * 1000000000ULL + now_ts.tv_nsec;
            latencies.push_back(now - out.timestamp_ns);

            if (ld == 0 || now - ld > 1000000000ULL) {
                std::cout << "\033[2J\033[H";

                std::cout << "=== IPC Inspector ===\n";
                std::cout << "Messages seen: " << count << "\n";
                std::cout << "Throughput:    " << msgs_this_second << " msg/sec\n";
                if (!latencies.empty()) {
                    std::sort(latencies.begin(), latencies.end());
                    std::cout << "p50= " << latencies[latencies.size() / 2] << "\n";
                    std::cout << "p90= " << latencies[latencies.size() * 9 / 10] << "\n";
                    std::cout << "p99= " << latencies[latencies.size() * 99 / 100] << "\n";
                } else {
                    std::cout << "p50= --\np90= --\np99= --\n";
                }
                std::cout << "Overflow:      " << dropped << "\n";
                msgs_this_second = 0;
                ld = now;
                latencies.clear();

            }


            
        }

    };
    sleep(10);
}