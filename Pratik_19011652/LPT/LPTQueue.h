#ifndef LPTQUEUE_H
#define LPTQUEUE_H

#include <QQueue>

namespace LPT {

template<class T> class LPTQueue
{
public:
    LPTQueue() {}
    // copy constructor
    LPTQueue(LPTQueue &other) : q(other.q) {}
    // Removes the head item in the queue and returns it. This function assumes that the queue isn't empty.
    inline T dequeue() {return q.dequeue();}
    // Adds value node to the tail of the queue.
    inline void enqueue(T node) {q.enqueue(node);}
    // Returns the queue's head item, but does not remove it from the queue. This function assumes that the queue isn't empty.
    inline T first() {return q.first();}
    // Returns true if the queue contains no items; otherwise returns false.
    inline bool isEmpty() {return q.isEmpty();}

private:
    QQueue<T> q;
};

}

#endif // LPTQUEUE_H
