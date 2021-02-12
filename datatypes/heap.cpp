#include "heap.h"

#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>

/* heap layout:
0
1         2
3   4     5     6
7 8 9 10  11 12 13 14
*/

#define parentIndex(i) ((i - 1)>>1)
#define leftChildIndex(i) ((i<<1) + 1)
#define rightChildIndex(i) ((i<<1) + 2)

typedef struct Heap {
  U32 count;
  U32 maxSize;
  HeapType type;
  HEAP_VALUE_TYPE values[1];
};

Heap *heapAlloc(HeapType type, U32 maxSize) {
  if (maxSize < 1) {
    throw std::length_error("heap maxSize must be at least 1");
  }
  Heap *heap = (Heap *)calloc(1, sizeof(Heap) + (maxSize - 1) * sizeof(HEAP_VALUE_TYPE));
  heap->type = type;
  heap->maxSize = maxSize;
  return heap;
}

void heapInsert(Heap *heap, HEAP_VALUE_TYPE value) {
  if (heap->count >= heap->maxSize) {
    throw std::overflow_error("heap is full");
  }

  U32 idx = heap->count++;
  heap->values[idx] = value;

  // sift up
  while (idx > 0) {
    U32 parentIdx = parentIndex(idx);
    HEAP_VALUE_TYPE parentValue = heap->values[parentIdx];
    HEAP_VALUE_TYPE diff = heap->type * (parentValue - value);
    if (diff >= 0) {
      // heap is valid
      return;
    }
    // swap
    heap->values[idx] = parentValue;
    heap->values[parentIdx] = value;
    idx = parentIdx;
  }
}

HEAP_VALUE_TYPE heapPeek(const Heap *heap) {
  if (!heap->count) {
    throw std::underflow_error("heap is empty");
  }
  return heap->values[0];
}

HEAP_VALUE_TYPE heapPop(Heap *heap) {
  if (!heap->count) {
    throw std::underflow_error("heap is empty");
  }

  const HEAP_VALUE_TYPE ret = heap->values[0];
  
  // remove last element and sift it down
  HEAP_VALUE_TYPE value = heap->values[--heap->count];
  for (U32 idx = 0; idx < heap->count;) {
    U32 leftIdx = leftChildIndex(idx);
    U32 rightIdx = rightChildIndex(idx);
    HEAP_VALUE_TYPE diffL = 0;
    HEAP_VALUE_TYPE diffR = 0;
    if (leftIdx < heap->count) {
      diffL = heap->type * (value - heap->values[leftIdx]);
    }
    if (leftIdx < heap->count) {
      diffR = heap->type * (value - heap->values[rightIdx]);
    }
    if (diffL >= 0 && diffR >= 0) {
      // heap is valid
      heap->values[idx] = value;
      break;
    }
    if (diffL < diffR) {
      heap->values[idx] = heap->values[leftIdx];
      idx = leftIdx;
    } else {
      heap->values[idx] = heap->values[rightIdx];
      idx = rightIdx;
    }
  }
  
  return ret;
}

U32 heapSize(const Heap *heap) {
  return heap->count;
}

void heapEmpty(Heap *heap) {
  heap->count = 0;
}

void heapPrint(const Heap *heap) {
  printf("Heap %d/%d\n", heap->count, heap->maxSize);
  U32 rowSize = 1;
  U32 rowCount = 0;
  for (U32 i = 0; i < heap->count; ++i) {
    printf("%.1f, ", heap->values[i]);
    rowCount++;
    if (rowCount >= rowSize) {
      printf("\n");
      rowCount = 0;
      rowSize = rowSize << 1;
    }
  }
  printf("\n\n");
}
