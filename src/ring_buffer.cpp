#include <atomic>
#include "ring_buffer.h"
#include <cstring>
#include <time.h>

const size_t capacity = 1000;

RingBuffer::RingBuffer() : write{0}, read{0}, seq{}, consumer_ready{false} {};


void RingBuffer::set_read(uint32_t new_read) {
    read.store(new_read, std::memory_order_release);
}

uint32_t RingBuffer::get_read() {
    return read.load(std::memory_order_acquire);
}

void RingBuffer::set_write(uint32_t new_write) {
    write.store(new_write, std::memory_order_release);
}

uint32_t RingBuffer::get_write() {
    return write.load(std::memory_order_acquire);
}

void RingBuffer::increment_seq(size_t idx) {
    seq[idx].fetch_add(1, std::memory_order_release);
}

bool RingBuffer::check_seq(size_t idx) {
    return ! (1 & seq[idx].load(std::memory_order_acquire));
}

uint32_t RingBuffer::get_seq(size_t idx) {
    return seq[idx].load(std::memory_order_acquire);
}

void RingBuffer::publish(const char* new_str){ 
    uint32_t wr = get_write();
    uint32_t re = get_read();
    if (wr - re == capacity) {
        // all unread slots are used 
        return;
    }

    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); 


    Message& msg = buffer[wr % capacity];
    
    seq[wr % capacity].fetch_add(1, std::memory_order_release);

    msg.timestamp_ns = ts.tv_sec * 1000000000ULL + ts.tv_nsec;
    memcpy(&(msg.data), (new_str), 8); 
    seq[wr % capacity].fetch_add(1, std::memory_order_release);
    set_write(wr + 1);
}

bool RingBuffer::consume(Message* out){
    uint32_t wr = get_write();
    uint32_t re = get_read();
    if (wr == re) {
        return false;
    }
    memcpy(out, &buffer[re % capacity], sizeof(Message)); 
    set_read(re + 1);
    return true;
}

Message RingBuffer::peek(uint32_t index) {
    return buffer[index % capacity];
}