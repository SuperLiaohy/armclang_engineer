//
// Created by Administrator on 24-10-3.
//

#ifndef DM_42_CUSTOMHEAP_H
#define DM_42_CUSTOMHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtx_lib.h"

#ifdef __cplusplus
}
#endif


class CustomHeap {
public:
    explicit CustomHeap(uint64_t *heap, uint32_t _size) : heap(heap) {
        osRtxMemoryInit(heap, _size);
        status = MemHeadPtr(heap);
    }

    void *malloc(uint32_t size);

    void free(void *ptr);

    uint32_t size() { return status->size; }

    uint32_t used() { return status->used; }

    uint32_t available() { return status->size - status->used; }

private:
    uint64_t *heap;
    mem_head_t *status;
};

extern CustomHeap DTCMHeap;
extern CustomHeap D1Heap;
extern CustomHeap D2Heap;
extern CustomHeap D3Heap;

#endif //DM_42_CUSTOMHEAP_H
