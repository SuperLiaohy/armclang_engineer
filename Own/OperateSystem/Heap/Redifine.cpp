#include "new"

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "stdio.h"

#ifdef __cplusplus
}
#endif

#include "CustomHeap.hpp"

#define USING_CUSTOM_D1_HEAP 1
#define USING_FREERTOS_HEAP 0

#if USING_CUSTOM_D1_HEAP == 1
void *operator new(size_t size) noexcept(false) {
    void *ptr = D1Heap.malloc(size);
    return ptr;
}

void operator delete(void *ptr) noexcept {
    D1Heap.free(ptr);
}

void *operator new[](size_t size) noexcept(false) {
    void *ptr = D1Heap.malloc(size);
    return ptr;
}

void operator delete[](void *ptr) noexcept {
    D1Heap.free(ptr);
}
#endif


#if USING_FREERTOS_HEAP == 1
void *operator new(size_t size) noexcept(false) {
    void *ptr = pvPortMalloc(size);
    return ptr;
}

void operator delete(void *ptr) noexcept {
    vPortFree(ptr);
}

void *operator new[](size_t size) noexcept(false) {
    void *ptr = pvPortMalloc(size);
    return ptr;
}

void operator delete[](void *ptr) noexcept {
    vPortFree(ptr);
}

void operator delete(void *ptr, size_t) noexcept {
    vPortFree(ptr);
}

void operator delete[](void *ptr, size_t) noexcept {
    vPortFree(ptr);
}
#endif