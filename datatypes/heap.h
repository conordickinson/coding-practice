#include "basics.h"

#define HEAP_VALUE_TYPE F32

enum HeapType {
  HEAPTYPE_MIN = -1,
  HEAPTYPE_MAX = 1,
};

struct Heap;

Heap *heapAlloc(HeapType type, U32 maxSize);
void heapInsert(Heap *heap, HEAP_VALUE_TYPE value);
HEAP_VALUE_TYPE heapPeek(const Heap *heap);
HEAP_VALUE_TYPE heapPop(Heap *heap);
U32 heapSize(const Heap *heap);
void heapEmpty(Heap *heap);
void heapPrint(const Heap *heap);
