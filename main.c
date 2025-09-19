#include "main.h"
#include "fft.h"

int main(int argc, char* argv[]) {

  cmplx_fa_t arr = initArray(8);

  for(size_t i = 0; i < 8; i ++){
    cmplx_f_t out = {i, 0};
    setElement(&arr, i, out);
  }

  printCmplx_fa(arr);

  fftSwap(&arr);
  
  printCmplx_fa(arr);

  

  return 0;
}
