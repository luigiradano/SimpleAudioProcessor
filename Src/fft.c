#include "../Inc/fft.h"


#include <stddef.h>
#include <stdint.h>


uint16_t reverseBit(uint16_t data, uint16_t size) {
  uint16_t out = 0;
  for (uint16_t i = 0; i < size; i++) {
    out |= ((data & 1 << i) >> i) << (size - i - 1);
  }
  return out;
}

void butterDivide(cmplx_fa_t* input, cmplx_fa_t* out, size_t offset, size_t size) {
  size_t j = 0;
  cmplx_f_t twiddle;
  for (size_t i = 0; i < size / 2; i++) {
    j = i + size / 2;
    twiddle = cExp(2.0f * PI * i / size);
    
    setElement(out, offset + i, sum(getElement(input, offset + i), getElement(input, offset + j)));
    setElement(out, offset + j, mul(twiddle, sub(getElement(input, offset + i), getElement(input, offset + j))));
  }
}

void butterCombine(cmplx_fa_t* input, cmplx_fa_t* out, size_t offset, size_t size) {
  size_t j = 0;
  cmplx_f_t twiddle;
  for (size_t i = 0; i < size / 2; i++) {
    j = i + size / 2;
    twiddle = cExp(-2.0f * PI * i / size);

    setElement(out, offset + i, sum(getElement(input, offset + i), mul(twiddle, getElement(input, offset + j))));
    setElement(out, offset + j, sub(getElement(input, offset + i), mul(twiddle, getElement(input, offset + j))));
  }
}

void computeFftButter(cmplx_fa_t input, cmplx_fa_t tmpArray, cmplx_fa_t output) {
  size_t bitCount = log(input.length) / log(2), layer = 1, currSize = 2;

  // Step 0: Allocate intermediate arrays
  cmplx_fa_t *interArray = NULL, out;
  interArray = malloc(sizeof(cmplx_fa_t) * (bitCount + 1));

  for (size_t i = 0; i < bitCount + 1; i++) {
    interArray[i] = initArray(input.length);
  }

  // Step 1: Reorder the input vector
  for (size_t i = 0; i < input.length; i++) {
    setElement(&interArray[0], i, getElement(&input, reverseBit(i, bitCount)));
#ifdef DEBUG_ORDER
    printf("Orig: %d\tDest: %d\n", i, reverseBit(i, bitCount));
#endif /* ifdef DEBUG */
  }

#ifdef DEBUG_LAYER
  printf("Layer %d\t Size %d\n", 0, 2);
  printCmplx_fa(interArray[0]);
#endif /* ifdef DEBUG */

  // Step 2: Go through the layers, perform butterfly combinations
  while (layer < bitCount + 1) {
    currSize = pow(2, layer);
    for (size_t i = 0; i < input.length / currSize; i++) {
      butterCombine(&interArray[layer - 1], &interArray[layer], i * currSize, currSize);
    }

#ifdef DEBUG_LAYER
    printf("Layer %d\t Size %d\n", layer, currSize);
    printCmplx_fa(interArray[layer]);
#endif /* ifdef DEBUG */
    layer++;
  }

  // Step 3: Free up the used memory, except for last intermediate array which is returned
  out = interArray[bitCount];

  for (size_t i = 0; i < bitCount; i++) {
    delArray(&interArray[i]);
  }
  free(interArray);

  return out;
}

cmplx_fa_t computeIFftButter(cmplx_fa_t input) {
  size_t bitCount = log(input.length) / log(2), layer = bitCount, currSize = 2;

  // Step 0: Allocate intermediate arrays
  cmplx_fa_t *interArray = NULL, out;
  interArray = malloc(sizeof(cmplx_fa_t) * (bitCount + 1));

  for (size_t i = 0; i < bitCount; i++) {
    interArray[i] = initArray(input.length);
  }
  interArray[bitCount] = input;

  // Step 1: Scale down by 1/N
  scaleArray(&input, 1.0f/input.length);

  // Step 1: Go through the layers, perform butterfly in opposite order
  while (layer > 0) {
    currSize = pow(2, layer);
    for (size_t i = 0; i < input.length / currSize; i++) {
      butterDivide(&interArray[layer], &interArray[layer - 1], i * currSize, currSize);
    }

#ifdef DEBUG_LAYER
    printf("Layer %d\t Size %d\n", layer, currSize);
    printCmplx_fa(interArray[layer]);
#endif /* ifdef DEBUG */

    layer--;
  }

  // Step 2: Scramble the factors back to the right index
  for (size_t i = 0; i < input.length; i++) {
    setElement(&interArray[bitCount], i, getElement(&interArray[0], reverseBit(i, bitCount)));
#ifdef DEBUG_ORDER
    printf("Orig: %d\tDest: %d\n", i, reverseBit(i, bitCount));
#endif /* ifdef DEBUG */
  }

  // Step 3: Free up the used memory, except for last intermediate array which is returned
  out = interArray[bitCount];

  for (size_t i = 0; i < bitCount; i++) {
    delArray(&interArray[i]);
  }
  free(interArray);

  return out;
}
