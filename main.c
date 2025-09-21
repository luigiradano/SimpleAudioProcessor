#include "main.h"
#include <stdint.h>
#include "fft.h"
#include "sdl_handle.h"

#define SIZE 2048

int main() {

  cmplx_fa_t out;
  Histogram_t hist;

  initPA(5, SIZE, 44100);
  initSDL(1920, 1080);

  setupHistogram(&hist, SIZE/2);

  startStream();

  while(!checkQuiteEvent()){
    out = computeFftButter(getBuff());

    uint64_t start = SDL_GetPerformanceCounter();

    for(size_t i = 0; i < SIZE/2; i ++){
      fillHistogram(&hist, cAbs(getElement(&out, i)), i);
    }

    drawHistogram(&hist);

    delArray(&out);
    
    uint64_t end = SDL_GetPerformanceCounter();

    float secondsElapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
    printf("FPS: %.2f Hz\n", 1/secondsElapsed);
  }

  return 0;
}
