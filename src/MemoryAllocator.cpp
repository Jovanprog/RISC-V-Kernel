#include "../h/MemoryAllocator.hpp"


 MemoryAllocator* MemoryAllocator::getInstance(){
    static MemoryAllocator instance;
    return &instance;
}

size_t MemoryAllocator::alignMemBlock(size_t size) {
     if (size == 0) return 0;

     if (size % MEM_BLOCK_SIZE == 0) return size;

     return size + (MEM_BLOCK_SIZE - size % MEM_BLOCK_SIZE);
}

size_t MemoryAllocator::alignedHeader() {
     return alignMemBlock(sizeof(MemBlock));
 }

//Vidi da li treba da alightujes i header to jest sizeof(MemBlock)

MemoryAllocator::MemoryAllocator(){
    //Inicijalizuj pocetak memorije na pocetak heap-a
    memBlockHead = (MemBlock*) HEAP_START_ADDR;

    //Size je bez velicine za MemBlock pokazivac
    memBlockHead->size = (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR - alignedHeader();
    memBlockHead->size -= memBlockHead->size % MEM_BLOCK_SIZE;
    memBlockHead->state = FREE;
    memBlockHead->prev = nullptr;
    memBlockHead->next = nullptr;
}

void *MemoryAllocator::mem_alloc(size_t size) {
    if(size <= 0) return nullptr;

    size_t alignedSize = alignMemBlock(size);

    MemBlock* temp = memBlockHead;

    while(temp){
        if(temp->state == FREE && temp->size >= alignedSize){
            size_t oldSize = temp->size;

            if(oldSize - alignedSize < alignedHeader() + MEM_BLOCK_SIZE){
                //Nadjen tacan blok, uzece se ceo blok
                temp->state = USED;
            }
            else{
                //Uzece se pocetni deo nadjenog slobodnog bloka
                MemBlock* newFree = (MemBlock*)((char*)temp + alignedHeader() + alignedSize);

                newFree->size = oldSize - alignedSize - alignedHeader();
                newFree->state = FREE;
                newFree->prev = temp;
                newFree->next = temp->next;

                if(temp->next != nullptr){
                    temp->next->prev = newFree;
                }

                temp->size = alignedSize;
                temp->state = USED;
                temp->next = newFree;
            }
            return (char*)temp + alignedHeader();
        }
        temp = temp->next;
    }
    return nullptr;
}

int MemoryAllocator::mem_free(void *block) {
    if(block == nullptr) return -1;

    MemBlock* freeBlock= (MemBlock*)((char*)block - alignedHeader());
    if(freeBlock->state == FREE) return -2;

    freeBlock->state = FREE;

    //Nalazenje prethodnog i sledeceg bloka i spajanje sa njima ako su FREE
    int spoji_sa = 0;

    if(freeBlock && freeBlock->prev && freeBlock->prev->state == FREE) spoji_sa = 1; //Spoji samo sa prev
    if(freeBlock && freeBlock->next && freeBlock->next->state == FREE){
        if(spoji_sa == 1) spoji_sa = 3; //Spoji i sa prev i sa next
        else spoji_sa = 2; //Spoji samo sa next
    }

    if(spoji_sa == 1){
        //Spoji samo sa prev
        MemBlock* prev = freeBlock->prev;
        MemBlock* next = freeBlock->next;

        prev->size += freeBlock->size + alignedHeader();
        prev->next = next;

        if(next){
            next->prev = prev;
        }
    }
    else if(spoji_sa == 2){
        //Spoji samo sa next
        MemBlock* prev = freeBlock->prev;
        MemBlock* next = freeBlock->next;

        freeBlock->size += next->size + alignedHeader();
        freeBlock->next = next->next;
        if(prev){
            prev->next = freeBlock;
        }
        else{
            memBlockHead = freeBlock;
        }

        if (next->next) {
            next->next->prev = freeBlock;
        }
    }
    else if(spoji_sa == 3){
        //Spoji sa prev i next
        MemBlock* prev = freeBlock->prev;
        MemBlock* next = freeBlock->next;

        prev->size += freeBlock->size + freeBlock->next->size + 2 * alignedHeader();
        prev->next = next->next;
        if(next->next){
            next->next->prev = prev;
        }
    }

    return 0;
}