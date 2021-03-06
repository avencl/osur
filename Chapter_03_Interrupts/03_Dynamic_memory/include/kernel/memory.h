/*! Memory management */
#pragma once

#include <types/basic.h>

void *k_mem_init(void *segment, size_t size);
void *kmalloc(size_t size);
int kfree(void *chunk);
