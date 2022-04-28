#include "queue.h"
//circalar queue

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
    if (front == -1 && rear == -1 ) result = true;
        
    return result;
}
// Adding an element
void Queue::enQueue(uint8_t element) {
    if (!isFull()) {
        if (front == -1) front = 0;
        rear = (rear + 1) % size;
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
            front = -1;
            rear = -1;
        }
        else {
            front = (front + 1) % size;
        }
    }
    return (element);
}
//set index not clear data
//so, next time write data will overwrite old data
void Queue::resetQueue(){
    front = -1;
    rear = -1;
}