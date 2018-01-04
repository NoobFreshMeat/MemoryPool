// StackAlloc.h

#ifndef STACK_ALLOC_H
#define STACK_ALLOC_H

#include <memory>

template <typename T>
struct StackNode
{
    T data;
    StackNode* prev;
};

template <class T, class Alloc = std::allocator<T> >
class StackAlloc
{
public:
    typedef StackNode<T> Node;
    typedef typename Alloc::template rebind<Node>::other Allocator;

    StackAlloc() {head = 0;}
    ~StackAlloc() {clear();}

    bool clear()
    {
        Node* curr = this->head;
        while (curr != 0)
        {
            Node* tmp = curr->prev;
            allocator.destroy(curr);//destroy the object that curr point to
            allocator.deallocate(curr, 1);//releaese the memory space  allocate for curr
            curr = tmp;
        }
        this->head = 0;
    }

    void push(T element)
    {
        Node* newNode = allocator.allocate(1);//arrange one Node type size memory space
        allocator.construct(newNode, Node());//construct Node object that newNode point to
        newNode->data = element;
        newNode->prev = head;
        this->head = newNode;
    }

    T pop()
    {
        T result = this->head->data;
        Node* tmp = this->head->prev;
        allocator.destroy(head);
        allocator.deallocate(head, 1);
        this->head = tmp;
        return result;
    }

    bool empty() { return (head == 0);}
    T top() { return head->data;}

private:
    Allocator allocator;
    Node* head;
};

#endif 
