#pragma once
#include "queue.h"
#include <math.h>

//circalar queue
//How enqueue and dequeue work if occurcue in sametime ??

Queue::~Queue() {
    delete items;
}
Queue::Queue(int s) {
    front = -1;
    rear = -1;
    size = s;
    items = new uint8_t[s];
}
// Check if the queue is full
bool Queue::isFull() {
    bool result = false;
    if (front == 0 && rear == size - 1) {
        result = true;
    }
    else if (front == rear + 1) {
        result = true;
    }

    return result;
}
// Check if the queue is empty
bool Queue::isEmpty() {
    bool result = false;
    if (front == -1 && rear == -1) result = true;
    return result;
}
// Adding an element
void Queue::enQueue(uint8_t element) {
    if (!isFull()) {
        if (front == -1 && rear == -1) {
            front = 0;
            rear = 0;
        }
        else
        {
            rear = (rear + 1) % size;
        }
        items[rear] = element;
    }
}
// Removing an element
uint8_t Queue::deQueue() {

    uint8_t element = 0;
    if (!isEmpty()) {
        element = items[front];
        //update front - rear
        if (front == rear) {
            // Q has only one element,
            // so we reset the queue after deleting it.
            resetQueue();
        }
        else {
            front = (front + 1) % size;
        }
    }
    return (element);
}
//find c in queue then return front
//if cant not find return -1
//make sure not dequeue after call findChar or result will not match
int Queue::findChar(char c) {
    int findNumber = c;
    int matchFront = -1; //frontIndex
    int count = -1;
    uint8_t element = 0;
    if (!isEmpty()) {
        matchFront = front;
        count = 0;
        while (matchFront != rear) {
            count++;
            if (findNumber == items[matchFront]) break;
            matchFront = (matchFront + 1) % size;
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
int Queue::capacity() {
    int result = 0;
    if (front == -1 && rear == -1) result = size;
    else if (front == 0 && rear == 0) result = size - 1;
    else if (front > rear) result = front - rear - 1;
    else if (front < rear) result = size - (rear - front + 1);
    return result;
}