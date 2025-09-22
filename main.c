#include "main.h"

#include <SDL2/SDL_timer.h>
#include <stddef.h>
#include <stdint.h>

#include "fft.h"
#include "sdl_handle.h"

#define SIZE_DEF 16
#define RB_TO_FFT_SIZE 3


int main(int argc, char* argv[]) {
  const uint32_t SAMPLE_RATE = 44100; // in Hz
  uint32_t fftBinsTotal = SIZE_DEF;
  float expectedCallbackFreq, maxVal;
  cmplx_fa_t outFft, input, outIfft, intermediate, convolute;
  cmplx_rb_t* inputRB, *outputRB;
  Histogram_t hist;

  // Initialize the program
  initSDL(1920, 1080);

  // Command line parameters
  if (argc > 2){
    fftBinsTotal = atoi(argv[2]);
    if(fftBinsTotal > getWinW()){
      printf("Too Large!\n");
    }
  }

  if (argc > 1)
    initPA(atoi(argv[1]), fftBinsTotal, fftBinsTotal*RB_TO_FFT_SIZE, SAMPLE_RATE);
  else
    initPA(5, fftBinsTotal, fftBinsTotal*RB_TO_FFT_SIZE, SAMPLE_RATE);


  setupHistogram(&hist, fftBinsTotal / 2);

  input = initArray(fftBinsTotal);
  convolute = initArray(fftBinsTotal);
  intermediate = initArray(fftBinsTotal);
  inputRB = getAudioInRingBuff();
  outputRB = getAudioOutRingBuff();

  expectedCallbackFreq = (float) SAMPLE_RATE / fftBinsTotal;

  for(size_t i = 0; i < fftBinsTotal; i ++){
    const float notchFreq = atoi(argv[3]);
    const float sigma = atof(argv[4]);
    float normFreq = notchFreq / (SAMPLE_RATE/2.0f);
    float modIdx = (float) i / fftBinsTotal;

    float value = pow(2, (-pow(modIdx - normFreq, 2)/(2*sigma*sigma)));

    if(value <= 0){
      value = 0;
    }
//    setElement(&convolute, i, (cmplx_f_t){(float)pow(10, modIdx)/10,0});
    setElement(&convolute, i, (cmplx_f_t){1, 0});
  }

  // Start the PortAudio callback
  startStream();

  while (!checkQuitEvent()) {

    // Read the data from the buffer
    getDataBuff(inputRB, &input);
    outFft = computeFftButter(input);
    mulArray(&outFft, &convolute, &intermediate);
    outIfft = computeIFftButter(intermediate);

    uint64_t start = SDL_GetPerformanceCounter();
    
    for(size_t i = 0; i < outFft.length; i ++){
      appendBuff(outIfft.data[i], outputRB);
    }
    // Draw the histogram
    for (size_t i = 0; i < fftBinsTotal / 2; i++) {
      //fillHistogramLog(&hist, cAbs(getElement(&out, i)), maxVal, i);
      fillHistogram(&hist, cAbs(getElement(&outFft, i)), i);
    }
    maxVal = hist.maxVal;

    drawHistogram(&hist);

    delArray(&outFft);

    uint64_t end = SDL_GetPerformanceCounter();
    
    float secondsElapsed = (end - start) / (float)SDL_GetPerformanceFrequency();

    // Print debug info
    printBuffHealth(inputRB);
    //printf("FPS: %.0fHz\t Max: %.2f\n", 1 / secondsElapsed, maxVal);
  }

  return 0;
}
