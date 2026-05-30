#include <atomic>
#include "ring_buffer_mutex.h"
#include <cstring>
#include <time.h>
#include <mutex>

const size_t capacity = 1000;


RingBuffer::RingBuffer() : write{0}, read{0}, consumer_ready{false} {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}



void RingBuffer::publish(const char* new_str){  
    pthread_mutex_lock(&mutex);
    uint32_t wr = write;
    uint32_t re = read;
    if (wr - re == capacity) {
        // all unread slots are used 
        return;
    }

    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); 


    Message& msg = buffer[wr % capacity];
    
    msg.timestamp_ns = ts.tv_sec * 1000000000ULL + ts.tv_nsec;
    memcpy(&(msg.data), (new_str), 8); 
    write = wr + 1;
    pthread_mutex_unlock(&mutex);

}

bool RingBuffer::consume(Message* out){
    pthread_mutex_unlock(&mutex);
    uint32_t wr = write;
    uint32_t re = read;
    if (wr == re) {
        return false;
    }
    memcpy(out, &buffer[re % capacity], sizeof(Message)); 
    read = re + 1;
    return true;
    pthread_mutex_unlock(&mutex);
}

uint32_t RingBuffer::get_write() {return write;}

Message RingBuffer::peek(uint32_t index) {
    return buffer[index % capacity];
}