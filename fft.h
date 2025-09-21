#ifndef FFT_INC
#define FFT_INC

#include "cmplx_math.h"

#define PI 3.141592653589793

cmplx_fa_t computeFftCustom(cmplx_fa_t* input) ;
void fftSwap(cmplx_fa_t* array, size_t size);

/**
*   @brief  Reverses the bits in the data passed as argument
*
*   @param  data  Number whose bits must be reversed
*   @param  size  How long is the number to be reversed in bits?
*/
uint16_t reverseBit(uint16_t data, uint16_t size);

cmplx_fa_t computeFftButter(cmplx_fa_t input);

cmplx_fa_t computeIFftButter(cmplx_fa_t input);
#endif  // !FFT_INC
