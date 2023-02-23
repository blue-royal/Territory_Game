#ifndef QUEUE__
#define QUEUE__

#include <vector>

template<typename T> class Queue
{
    public:
        Queue() = default;
        Queue(unsigned int max_len) : max_size(max_len);
        void enqueue(T new_obj);
        void dequeue();
        T peek();
        bool is_empty();
        bool is_full();

    private:
        unsigned int max_size = 10;
        std::vector<T> queue;
};

#endif