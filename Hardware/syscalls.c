#include <errno.h>
#include <stdint.h>

extern uint8_t _end[];
extern uint8_t _estack[];
static uint8_t *heap_end = _end;

void *_sbrk(intptr_t increment) {
    uint8_t *prev_heap_end = heap_end;

    if (heap_end + increment > _estack) {
        errno = ENOMEM;
        return (void *)-1;
    }

    heap_end += increment;
    return (void *)prev_heap_end;
}
