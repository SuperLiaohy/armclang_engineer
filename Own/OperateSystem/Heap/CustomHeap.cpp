//
// Created by Administrator on 24-10-3.
//

#include "CustomHeap.hpp"

void *CustomHeap::malloc(uint32_t len) {
    return osRtxMemoryAlloc(heap, len, 0);
}

void CustomHeap::free(void *ptr) {
    osRtxMemoryFree(heap, ptr);
}

