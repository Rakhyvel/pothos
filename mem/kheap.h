#ifndef KHEAP_H
#define KHEAP_H

#include "../lib/system.h"

#define KHEAP_START         (void*)0xC0400000
#define KHEAP_INITIAL_SIZE  48 * M
#define KHEAP_MAX_ADDRESS   (void*)0xCFFFFFFF
#define HEAP_MIN_SIZE       4 * M

#endif