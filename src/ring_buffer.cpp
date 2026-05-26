#include <atomic>
#include "ring_buffer.h"

RingBuffer::RingBuffer() : write{0}, read{0} {};