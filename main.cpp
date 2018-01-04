// MemoryPool

#include <iostream>
#include <cassert>
#include <ctime>
#include <vector>

#include "MemoryPool.h"
#include "StackAlloc.h"

#define ELEMS 1000000
#define REPS 10

int main()
{
    clock_t start;
    StackAlloc<int, std::allocator<int> > stackDefault;
    start = clock();
    for (int j = 0; j < REPS; j++)
    {
        assert(stackDefault.empty());
        for (int i = 0; i < ELEMS; i++)
        {
            stackDefault.push(i);
        }
        for (int i = 0; i < ELEMS; i++)
        {
            stackDefault.pop();
        }
    }
    std::cout << "Default Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

    StackAlloc<int, MemoryPool<int> > stackPool;    
    start = clock();
    for (int j = 0; j < REPS; j++)
    {
        assert(stackPool.empty());
        for (int i = 0; i < ELEMS; i++)
        {
            stackPool.push(i);
        }
        for (int i = 0; i < ELEMS; i++)
        {
            stackPool.pop();
        }
    }
    std::cout << "MemoryPool Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";
   
    std::vector<int> stackVector; 
    start = clock();
    for (int j = 0; j < REPS; j++)
    {
        assert(stackVector.empty());
        for (int i = 0; i < ELEMS; i++)
        {
            stackVector.push_back(i);
        }
        for (int i = 0; i < ELEMS; i++)
        {
            stackVector.pop_back();
        }
    }
    std::cout << "Vector Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

    
    return 0;
}
