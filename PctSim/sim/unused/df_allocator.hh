#ifndef DF_ALLOCATOR

#if (__GNUC__ == 2 && __GNUC_MINOR__ == 95) || \
    (__GNUC__ == 3 && __GNUC_MINOR__ == 2)

#define DF_ALLOCATOR thread_allocator
#include "thread_allocator/thread_allocator.h"

#else

#define DF_ALLOCATOR std::allocator

#endif

#endif /* DF_ALLOCATOR */