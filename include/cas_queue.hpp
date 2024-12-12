#include "task.hpp"
#include <atomic>

template <typename T>
class CasQueue
{
private:
    struct casQueueNode
    {
        T value;
        std::atomic<casQueueNode *> next;
        casQueueNode(T value) : value(value), next(nullptr) {};
    };

    std::atomic<casQueueNode *> head;
    std::atomic<casQueueNode *> tail;

public:
    CasQueue()
    {
        T value;
        casQueueNode *dummy = new casQueueNode(value);
        head.store(dummy);
        tail.store(dummy);
    }
    ~CasQueue() {};

    void enqueue(T value)
    {
        casQueueNode *oldTail;
        casQueueNode *newTail = new casQueueNode(value);

        while (true)
        {
            oldTail = tail.load();
            casQueueNode *next = oldTail->next.load();
            if (oldTail == tail.load())
            {
                if (next == nullptr)
                {
                    if (oldTail->next.compare_exchange_weak(next, newTail))
                    {
                        break;
                    }
                }
                else
                {
                    tail.compare_exchange_weak(oldTail, next);
                }
            }
        }
        tail.compare_exchange_weak(oldTail, newTail);
    }

    bool dequeue(T &value)
    {
        casQueueNode *oldHead;

        while (true)
        {
            oldHead = head.load();
            casQueueNode *next = oldHead->next.load();
            if (oldHead == head.load())
            {
                if (next == nullptr)
                    return false;

                if (head.compare_exchange_weak(oldHead, next))
                {
                    value = next->value;
                    break;
                }
            }
        }
        delete oldHead;
        return true;
    }

    bool checkSize(size_t size)
    {
        casQueueNode *currentNode = head.load();
        while (size--)
        {
            if (currentNode->next == nullptr)
            {
                return false;
            }
            else
            {
                currentNode = currentNode->next.load();
            }
        }
        return true;
    }
};
