#include "main.h"

#include <SDL2/SDL_timer.h>
#include <stdint.h>

#include "fft.h"
#include "sdl_handle.h"

#define SIZE_DEF 1024
#define RB_TO_FFT_SIZE 3


int main(int argc, char* argv[]) {
  const uint32_t SAMPLE_RATE = 44100; // in Hz
  uint32_t fftBinsTotal = SIZE_DEF;
  float expectedCallbackFreq, maxVal;
  cmplx_fa_t out, input;
  cmplx_rb_t* audioRB;
  Histogram_t hist;

  // Command line parameters
  if (argc > 2)
    fftBinsTotal = atoi(argv[2]);

  if (argc > 1)
    initPA(atoi(argv[1]), fftBinsTotal, fftBinsTotal*RB_TO_FFT_SIZE, SAMPLE_RATE);
  else
    initPA(5, fftBinsTotal, fftBinsTotal*RB_TO_FFT_SIZE, SAMPLE_RATE);

  // Initialize the program
  initSDL(1920, 1080);

  setupHistogram(&hist, fftBinsTotal / 2);

  input = initArray(fftBinsTotal);
  audioRB = getAudioRingBuff();
  expectedCallbackFreq = (float) SAMPLE_RATE / fftBinsTotal;

  // Start the PortAudio callback
  startStream();

  while (!checkQuiteEvent()) {

    // Read the data from the buffer
    getDataBuff(audioRB, &input);
    out = computeFftButter(input);

    uint64_t start = SDL_GetPerformanceCounter();
    
    // Draw the histogram
    for (size_t i = 0; i < fftBinsTotal / 2; i++) {
      //fillHistogramLog(&hist, cAbs(getElement(&out, i)), maxVal, i);
      fillHistogram(&hist, cAbs(getElement(&out, i)), i);
    }
    maxVal = hist.maxVal;

    drawHistogram(&hist);

    delArray(&out);

    SDL_Delay(990.0f/(expectedCallbackFreq));
    uint64_t end = SDL_GetPerformanceCounter();
    
    float secondsElapsed = (end - start) / (float)SDL_GetPerformanceFrequency();

    // Print debug info
    printBuffHealth(audioRB);
    //printf("FPS: %.0fHz\t Max: %.2f\n", 1 / secondsElapsed, maxVal);
  }

  return 0;
}
