#include "queue.h"

template<typename T>
void Queue<T>::enqueue(T new_obj){
    if(!is_full()){
        queue.push_back(new_obj);
        std::rotate(queue.rbegin(), queue.rbegin() + 1, queue.rend());
    }
}

template<typename T>
void Queue<T>::dequeue(){
    queue.pop_back();
}

template<typename T>
bool Queue<T>::is_empty(){
    if(queue.size() == 0){
        return true;
    }
    return false;
}
template<typename T>
bool Queue<T>::is_full(){
    if(queue.size() >= max_size){
        return true;
    }
    return false;
}

template<typename T>
T Queue<T>::peek(){
    return *(queue.begin() + queue.size());
}