#include "queue.h"

template<typename T>
void Queue<T>::enqueue(T new_obj){
    // if not full add fromt of queue
    if(!is_full()){
        // put new object at the back
        queue.push_back(new_obj);
        // rotate so the last element becomes the first element
        std::rotate(queue.rbegin(), queue.rbegin() + 1, queue.rend());
    }
}

template<typename T>
void Queue<T>::dequeue(){
    // remove last element from array
    queue.pop_back();
}

template<typename T>
bool Queue<T>::is_empty(){
    // check the size of the vector
    if(queue.size() == 0){
        return true;
    }
    return false;
}
template<typename T>
bool Queue<T>::is_full(){
    // check size of vector
    if(queue.size() >= max_size){
        return true;
    }
    return false;
}

template<typename T>
T Queue<T>::peek(){
    // return the last element of vector
    return *(queue.begin() + queue.size());
}