#include "fft.h"


#include <stddef.h>
#include <stdint.h>

void baseFft(cmplx_fa_t* input, cmplx_fa_t* out, int offset) {
  cmplx_f_t a, b;
  a = sum(input->data[0 + offset], input->data[1 + offset]);
  b = sub(input->data[0 + offset], input->data[1 + offset]);

  setElement(out, 0 + offset, a);
  setElement(out, 1 + offset, b);
}

void combineFft(cmplx_fa_t* input, cmplx_fa_t* out, size_t offset, size_t size) {
  size_t j = 0;
  for (size_t i = 0; i < size / 2; i++) {
    j = i + size / 2;
    setElement(out, offset + i, sum(getElement(input, offset + i), mul(cExp(-2.0f * PI * i / size), getElement(input, offset + i + size / 2))));
    setElement(out, offset + j, sum(getElement(input, offset + i), mul(cExp(-2.0f * PI * j / size), getElement(input, offset + i + size / 2))));
  }
}

cmplx_fa_t computeFftCustom(cmplx_fa_t* input) {
  cmplx_fa_t out;
  int layerMax = log(input->length) / log(2), layer = 1, currSize = 2;

  if ((input->length > 0 && ((input->length & (input->length - 1)) != 0))) {
    fprintf(stderr, "Error computing FFT, invalid size!\n");
    exit(1);
  }

  out = initArray(input->length);

  // Main algorithm for non trivial case
  fftSwap(input, input->length);  // Put even indeces at start, odd ones at the end

  printf("Layer %d\t Size %d\n", 0, 2);
  printCmplx_fa(*input);

  // Apply layer 0 fft to each couple of elements
  for (size_t i = 0; i < input->length; i += 2) {
    baseFft(input, input, i);
  }

  printf("Layer %d\t Size %d\n", 1, 2);
  printCmplx_fa(*input);

  while (layer < layerMax) {
    layer++;
    currSize = pow(2, layer);
    printf("Layer %d\t Size %d\n", layer, currSize);
    for (size_t i = 0; i < input->length / currSize; i++) {
      if (layer % 2 == 0) {
        combineFft(input, &out, i * currSize, currSize);
      } else {
        combineFft(&out, input, i * currSize, currSize);
      }
    }
    if (layer % 2 == 0) {
      printCmplx_fa(*input);
    } else {
      printCmplx_fa(out);
    }
  }

  if (layer % 2 == 0)
    return out;
  else
    return *input;
}

void fftSwap(cmplx_fa_t* array, size_t size) {
  for (size_t i = 1; i < size / 2; i++) {
    moveToIdx(array, 2 * i, i);
  }
}

uint16_t reverseBit(uint16_t data, uint16_t size) {
  uint16_t out = 0;
  for (uint16_t i = 0; i < size; i++) {
    out |= ((data & 1 << i) >> i) << (size - i - 1);
  }
  return out;
}

void butterCombine(cmplx_fa_t* input, cmplx_fa_t* out, size_t offset, size_t size) {
  size_t j = 0;
  cmplx_f_t twiddle;
  for (size_t i = 0; i < size / 2; i++) {
    j = i + size / 2;
    twiddle = cExp(-2.0f * PI * i / size);

    setElement(out, offset + i, sum(getElement(input, offset + i), mul(twiddle, getElement(input, offset + i + size / 2))));
    setElement(out, offset + j, sub(getElement(input, offset + i), mul(twiddle, getElement(input, offset + i + size / 2))));
  }
}

cmplx_fa_t computeFftButter(cmplx_fa_t input) {
  size_t bitCount = log(input.length) / log(2), layer = 1, currSize = 2;

  // Step 0: Allocate intermediate arrays
  cmplx_fa_t *interArray = NULL, out;
  interArray = malloc(sizeof(cmplx_fa_t) * (bitCount+1));

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

  for (int i = 0; i < bitCount; i++) {
    delArray(&interArray[i]);
  }
  free(interArray);

  return out;
}
