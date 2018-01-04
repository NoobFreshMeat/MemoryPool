//MemoryPool.h

#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include <climits>
#include <cstddef>

template <typename T, size_t BlockSize = 4096>
class MemoryPool
{
public:
    typedef T* pointer;

    template <typename U> 
    struct rebind
    {
        typedef MemoryPool<U> other;
    };

    MemoryPool() noexcept
    {
        currentBlock = nullptr;  
        currentSlot = nullptr;
        lastSlot = nullptr;
        freeSlots = nullptr;
    }

    ~MemoryPool() noexcept
    {
        slot_pointer curr = currentBlock;
        while (curr != nullptr) // delete all memory slot
        {
            slot_pointer prev = curr->next;
            operator delete(reinterpret_cast<void*>(curr));
            curr = prev;
        }
    }

    pointer allocate(size_t n = 1, const T* hint = 0)
    {
        if (freeSlots != nullptr)
        {
            pointer result = reinterpret_cast<pointer>(freeSlots);
            freeSlots = freeSlots->next;
            return result;
        }
        else if (currentSlot >= lastSlot)
        { 
            // apply for a new memory block from top of current block
            data_pointer newBlock 
                = reinterpret_cast<data_pointer>(operator new(BlockSize));
            reinterpret_cast<slot_pointer>(newBlock)->next = currentBlock;
            currentBlock = reinterpret_cast<slot_pointer>(newBlock);
            // uintptr_t: unsigned int type which can store a pointer
            // alignof: return memory align requirement
            data_pointer body = newBlock + sizeof(slot_pointer);
            uintptr_t result = reinterpret_cast<uintptr_t>(body);
            size_t bodyPadding = (alignof(slot_type) - result) % alignof(slot_type);
            currentSlot = reinterpret_cast<slot_pointer>(body + bodyPadding);
            lastSlot = reinterpret_cast<slot_pointer>(newBlock + BlockSize - 
                sizeof(slot_type));
       
            return reinterpret_cast<pointer>(currentSlot++);
        }
     }
            

    void deallocate(pointer p, size_t n =1)
    {
        if (p != nullptr)
        {
            reinterpret_cast<slot_pointer>(p)->next = freeSlots;
            freeSlots = reinterpret_cast<slot_pointer>(p);
        }
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args)
    {
        new (p) U (std::forward<Args>(args)...); //placement new
    }   


    template <typename U>
    void destroy(U* P)
    {
        P->~U();
    }

private:
    union Slot
    {
       //store only data or only pointer
       T element;
       Slot* next;
    };

    typedef char* data_pointer;
    typedef Slot slot_type;
    typedef Slot* slot_pointer;  

    slot_pointer currentBlock; //point to current memory block (contain memory slot)
    slot_pointer currentSlot; //point to current memory slot (in current memory block) 
    slot_pointer lastSlot; // point to last memory slot in current memory block
    slot_pointer freeSlots; //point to curret free memory slot in current memory block

    static_assert(BlockSize >= 2 * sizeof(slot_type), "BlockSize too samll.");
};
#endif

