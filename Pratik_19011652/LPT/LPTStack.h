#ifndef LPTSTACK_H
#define LPTSTACK_H

#include <QStack>

namespace LPT {

template<class T> class LPTStack
{
public:
    LPTStack() {};
    // copy constructor
    LPTStack(LPTStack &other) : stack(other.stack) {}
    // Removes the top item from the stack and returns it. This function assumes that the stack isn't empty.
    T pop() {return stack.pop();}
    // Adds element n to the top of the stack.
    void push(T n) {stack.push(n);}
    // Returns the stack's top item, but does not remove it. This function assumes that the stack isn't empty.
    T top() {return stack.top();}
    // Returns true if the stack contains no items; otherwise returns false.
    bool isEmpty() {return stack.isEmpty();}

private:
    QStack<T> stack;
};

}
#endif // LPTNODESTACK_H
