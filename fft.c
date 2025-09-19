#include "fft.h"

cmplx_fa_t computeFft(cmplx_fa_t input) {
  cmplx_fa_t out;

  if ((input.length > 0 && ((input.length & (input.length - 1)) == 0))){
    fprintf(stderr, "Error computing FFT, invalid size!\n");
    exit(1);
  }

  out = initArray(input.length);

}

void fftSwap(cmplx_fa_t* array){

  for(size_t i = 1; i < array->length/2; i ++){
    moveToIdx(array, 2*i, i);
  }
}

