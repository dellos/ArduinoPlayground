#pragma once
#include <stdint.h>
#include <util/atomic.h>

class Queue {
private:
    int8_t front, rear, size;
    uint8_t* items;
public:
    Queue(int8_t size);
    ~Queue();
    void enQueue(uint8_t value);
    uint8_t deQueue();
    int8_t findChar(char value);
    bool isFull();
    bool isEmpty();
    void resetQueue();
    int8_t capacity();
};