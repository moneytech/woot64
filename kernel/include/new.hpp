#pragma once

// new with alignment
void *operator new(size_t size, size_t alignment);
void *operator new[](size_t size, size_t alignment);

// placement new
void *operator new(size_t size, void *ptr);
void *operator new[](size_t size, void *ptr);
