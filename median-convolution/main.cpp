#include "../datatypes/basics.h"
#include "../datatypes/heap.cpp"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void medianInsert(Heap *leftHeap, Heap *rightHeap, F32 value) {
  U32 leftSize = heapSize(leftHeap);
  U32 rightSize = heapSize(rightHeap);

  if (leftSize && value < heapPeek(leftHeap)) {
    heapInsert(leftHeap, value);

    // note: leftSize is now off by one because of the insert, but that is compensated for in the expression below, which would normally be (leftSize > rightSize + 1)
    if (leftSize > rightSize) {
      heapInsert(rightHeap, heapPop(leftHeap));
    }
  } else if (rightSize && value > heapPeek(rightHeap)) {
    heapInsert(rightHeap, value);

    // note: rightSize is now off by one because of the insert, but that is compensated for in the expression below, which would normally be (rightSize > leftSize + 1)
    if (rightSize > leftSize) {
      heapInsert(leftHeap, heapPop(rightHeap));
    }
  } else if (leftSize < rightSize) {
    heapInsert(leftHeap, value);
  } else {
    heapInsert(rightHeap, value);
  }
}

// median convolution of f32 heightfield
F32 *heightfieldMedian(F32 *heightfield, U32 hWidth, U32 hHeight, U32 convR) {
  F32 *medians = (F32 *)malloc(hWidth * hHeight * sizeof(F32));

  const int convD = 1 + convR + convR;
  Heap *leftHeap = heapAlloc(HEAPTYPE_MAX, convD * convD);
  Heap *rightHeap = heapAlloc(HEAPTYPE_MIN, convD * convD);
  
  for (int x = 0; x < hWidth; ++x) {
    for (int y = 0; y < hHeight; ++y) {
      heapEmpty(leftHeap);
      heapEmpty(rightHeap);
      
      int fx1 = fmax(0, x - convR);
      int fx2 = fmin(hWidth - 1, x + convR);
      int fy1 = fmax(0, y - convR);
      int fy2 = fmin(hHeight - 1, y + convR);
      for (int sx = fx1; sx <= fx2; ++sx) {
        for (int sy = fy1; sy <= fy2; ++sy) {
          medianInsert(leftHeap, rightHeap, heightfield[sy * hWidth + sx]);
        }
      }
      medians[y * hWidth + x] = heapSize(leftHeap) > heapSize(rightHeap) ? heapPeek(leftHeap) : heapPeek(rightHeap);
    }
  }
  
  free(leftHeap);
  free(rightHeap);
  
  return medians;
}

int main(void) {
  F32 heightfield[] = {
    1, 30, 20, -1, 3,
    8, 40, 29, 0, 2,
    -2, 33, 109, 39, 1,
    17, 2, 1, 76, 4,
    1, 1, 2, 3, 5
  };
  
  F32 *medians = heightfieldMedian(heightfield, 5, 5, 2);
  for (int y = 0; y < 5; ++y) {
    printf("%.1f, %.1f, %.1f, %.1f, %.1f\n", medians[y * 5 + 0], medians[y * 5 + 1], medians[y * 5 + 2], medians[y * 5 + 3], medians[y * 5 + 4]);
  }
  free(medians);

  return 0;
}
