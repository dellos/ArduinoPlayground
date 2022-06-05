#pragma once
#include "queue.h"
#include <math.h>

//circalar queue
//How enqueue and dequeue work if occurcue in sametime ??

Queue::~Queue() {
    delete[] items;
}
Queue::Queue(int8_t s) {
    front = -1;
    rear = -1;
    size = s;
    items = new uint8_t[s];
}
// Check if the queue is full
bool Queue::isFull() {
    int f , r ,s;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        f = front;
        r = rear;
        s = size;
    }
    bool result = false;  
    if (f == 0 && r == s - 1) {
        result = true;
    }
    else if (f == r + 1) {
        result = true;
    }
    return result;
}
// Check if the queue is empty
bool Queue::isEmpty() {
    bool result = false;
    int f , r ,s;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        f = front;
        r = rear;
        s = size;
    }
    if (f == -1 && r == -1) result = true;
    return result;
}
// Adding an element
void Queue::enQueue(uint8_t element) {
    int f , r ,s;
    int tmpRear;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        f = front;
        r = rear;
        s = size;
    }
    if (!isFull()) {
        if (f == -1 && r == -1) {
            front = 0;
            rear = 0;
            tmpRear = 0;
        }
        else
        {
            tmpRear = (r + 1) % s;
            rear = tmpRear;
        }
        items[tmpRear] = element;
    }
}
// Removing an element
uint8_t Queue::deQueue() {

    uint8_t element = 0;
    int f , r ,s;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        f = front;
        r = rear;
        s = size;
    }
    if (!isEmpty()) {
        element = items[f];
        //update front - rear
        if (f == r) {
            // Q has only one element,
            // so we reset the queue after deleting it.
            resetQueue();
        }
        else {
            front = (f + 1) % s;
        }
    }
    return (element);
}
//find c in queue then return front
//if cant not find return -1
//make sure not dequeue after call findChar or result will not match
int8_t Queue::findChar(char c) {
    int findNumber = c;
    int matchFront = -1; //frontIndex
    int count = -1;
    uint8_t element = 0;
    int f , r ,s;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        f = front;
        r = rear;
        s = size;
    }
    if (!isEmpty()) {
        matchFront = f;
        count = 0;
        while (matchFront != r) {
            count++;
            if (findNumber == items[matchFront]) break;
            matchFront = (matchFront + 1) % s;
        }
    }
    return count;
}

//set index not clear data
//so, next time write data will overwrite old data
void Queue::resetQueue() {
    front = -1;
    rear = -1;
}
//return not using bytes
int8_t Queue::capacity() {
    int f , r ,s;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
        f = front;
        r = rear;
        s = size;
    }
    int result = 0;
    if (f == -1 && r == -1) result = s;
    else if (f == 0 && r == 0) result = s - 1;
    else if (f> r) result = f - r - 1;
    else if (f < r) result = s - (r - f + 1);
    return result;
}