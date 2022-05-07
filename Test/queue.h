#pragma once
#include <stdint.h>


class Queue {
private:
    int front, rear, size;
    uint8_t* items;
public:
    Queue(int size);
    ~Queue();
    void enQueue(uint8_t value);
    uint8_t deQueue();
    int findChar(char value);
    bool isFull();
    bool isEmpty();
    void resetQueue();
    int capacity();
};