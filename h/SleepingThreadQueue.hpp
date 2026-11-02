#ifndef PROJECT_BASE_V1_1_SLEEPINGTHREADQUEUE_HPP
#define PROJECT_BASE_V1_1_SLEEPINGTHREADQUEUE_HPP

class TCB;

class SleepingThreadQueue {
public:
    SleepingThreadQueue() :  head(nullptr), tail(nullptr), num(0) {}

    static void* operator new(size_t size){
        return MemoryAllocator::getInstance()->mem_alloc(size);
    }
    static void operator delete(void* ptr){
        if (ptr != nullptr) {
            MemoryAllocator::getInstance()->mem_free(ptr);
        }
    }
    static void operator delete(void* ptr, size_t size){
        if (ptr != nullptr) {
            MemoryAllocator::getInstance()->mem_free(ptr);
        }
    }

    SleepingThreadQueue &operator= (const SleepingThreadQueue &) = delete;

    void add(TCB *data, uint64 timeToSleep){
        Elem *elem = new Elem(data, timeToSleep, head);
        num++;

        if(!tail){
            head = tail = elem;
            return;
        }

        if (timeToSleep < head->timeToSleep) {
            elem->next = head;
            head = elem;
            return;
        }

        Elem* prev = head;
        Elem* curr = head->next;

        while (curr && curr->timeToSleep <= timeToSleep) {
            prev = curr;
            curr = curr->next;
        }

        elem->next = curr;
        prev->next = elem;
        if (!curr) {
            tail = elem;
        }
    }


    TCB *remove(){
        if (!head) return nullptr;

        Elem* elem = head;
        head = head->next;
        num--;

        //Poslednji sklonjen
        if (!head) {
            tail = nullptr;
        }

        TCB* tcb = elem->data;
        delete elem;
        return tcb;
    }

    TCB *peekFirst() const {
        if(!head) return nullptr;
        return head->data;
    }

    uint64 peekFirstTime() const {
        if(!head) return -1;
        return head->timeToSleep;
    }

    TCB *peekLast() const {
        if(!tail) return nullptr;
        return tail->data;
    }

    uint64 getNum() const {
        return num;
    }

private:
    struct Elem
    {
        TCB* data;
        Elem* next;
        uint64 timeToSleep;

        Elem(TCB* data, uint64 timeToSleep, Elem* next) : data(data), next(next), timeToSleep(timeToSleep) {}

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
    uint64 num;
};

#endif //PROJECT_BASE_V1_1_SLEEPINGTHREADQUEUE_HPP
