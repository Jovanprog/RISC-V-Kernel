#ifndef PROJECT_BASE_V1_1_QUEUE_HPP
#define PROJECT_BASE_V1_1_QUEUE_HPP

#include "../h/MemoryAllocator.hpp"

template<typename T>
class Queue {
public:
    static void* operator new(size_t size){
        return MemoryAllocator::getInstance()->mem_alloc(size);
    }
    static void operator delete(void* ptr){
        if (ptr != nullptr) {
            MemoryAllocator::getInstance()->mem_free(ptr);
        }
    }

    Queue() :  head(nullptr), tail(nullptr) {}

    ~Queue() {
       while (head != nullptr) {
           Elem* old = head;
           head = head->next;
           delete old;
       }

       tail = nullptr;
   }

   Queue<T> &operator= (const Queue<T> &) = delete;

   void addFirst(T *data){
       Elem *elem = new Elem(data, head);
       head = elem;
       if(!tail){
           tail = head;
       }
   }

   void addLast(T *data){
       Elem *elem = new Elem(data, 0);
       if(tail){
           tail->next = elem;
           tail = elem;
       }
       else{
           head = tail = elem;
       }
   }

   T *removeFirst(){
       if(!head) return nullptr;

       Elem *elem = head;
       head = head->next;
       if(!head) tail = 0;

       T *ret = elem->data;
       delete elem;
       return ret;
   }

   T *peekFirst(){
       if(!head) return nullptr;
       return head->data;
   }

   T *removeLast(){
       if(!head) return nullptr;

       Elem *prev = nullptr;
       for(Elem *curr = head; curr && curr != tail; curr = curr->next){
           prev = curr;
       }

       Elem *elem = tail;
       if(prev) prev->next = 0;
       else head = 0;
       tail = prev;

       T *ret = elem->data;
       delete elem;
       return ret;
   }

   T *peekLast(){
       if(!tail) return nullptr;
       return tail->data;
   }

private:
    struct Elem
    {
       T* data;
       Elem* next;

       Elem(T* data, Elem* next) : data(data), next(next) {}

        static void* operator new(size_t size){
            return MemoryAllocator::getInstance()->mem_alloc(size);
        }
        static void operator delete(void* ptr){
            if (ptr != nullptr) {
                MemoryAllocator::getInstance()->mem_free(ptr);
            }
        }
    };

   Elem *head, *tail;
};


#endif //PROJECT_BASE_V1_1_QUEUE_HPP
