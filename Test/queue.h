#pragma once
#include <stdint.h>

class Queue{
   private:
    int front, rear, size; 
    uint8_t *items;

   public:
    Queue(int size) {
        front = -1;
        rear = -1;
        items = new uint8_t[size];
    }
    void enQueue(uint8_t value);
    uint8_t deQueue();
    bool isFull();
    bool isEmpty();
    void resetQueue();
};