#include "sdl_handle.h"

//#define DEBUG_AG

SDL_Window* win = NULL;
SDL_Surface* screenSurf = NULL;
SDL_Renderer* rend = NULL;
uint32_t winW, winH;

void initSDL(uint32_t width, uint32_t height) {
  winW = width;
  winH = height;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Error initializing SDL!\n");
    exit(2);
  }

  win = SDL_CreateWindow("FFT from scratch",
                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                         width, height,
                         SDL_WINDOW_SHOWN);

  if (win == NULL) {
    fprintf(stderr, "Error opening SDL window!\n");
    exit(2);
  }

  // screenSurf = SDL_GetWindowSurface(win);

  rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void quitSDL() {
  SDL_DestroyWindow(win);
  SDL_Quit();
}

SDL_Window* getWinSDL() {
  return win;
}

SDL_Surface* getSurfSDL() {
  return screenSurf;
}

SDL_Renderer* getRendSDL() {
  return rend;
}

uint32_t getWinH() {
  return winH;
}
uint32_t getWinW() {
  return winH;
}

void setupHistogram(Histogram_t* hist, size_t fullLen) {
  hist->vertScale = 20;
  hist->autoScaleFact = 1;
  hist->sumRectW = 0;
  hist->underflow = 0;
  hist->overflow = 0;
  hist->maxVal = 0;
  hist->minVal = 0;
  hist->avgVal = 0;
  hist->offset = 0.4;
  hist->fullLen = fullLen;
  hist->rects = (SDL_Rect*)malloc(sizeof(SDL_Rect) * fullLen);
}

void fillHistogramDb(Histogram_t* hist, float value, float maxValue, size_t index) {
  fillHistogram(hist, 10 * log10(value) / log10(maxValue), index);
}

// Returns the new max value in the input data
void fillHistogramLog(Histogram_t* hist, float value, float maxVal, size_t index) {
  float tmp = value / maxVal;
  if (tmp > 0 && !isinf(tmp))
    fillHistogram(hist, hist->offset + log10(tmp), index);

  if (!isinf(value) > hist->maxVal)
    hist->maxVal = value;
  if (!isinf(value) < hist->minVal)
    hist->minVal = value;

  hist->avgVal += value;

  if (index == hist->fullLen - 1) {  // On last element compute the average
    hist->avgVal /= (float)hist->fullLen;
  }
}

void fillHistogram(Histogram_t* hist, float value, size_t index) {
  uint16_t rectH, rectW;

  if (index == 0) {
    hist->sumRectW = 0;
    hist->avgVal = 0;
    hist->minVal = value;
    hist->maxVal = value;


    int autoGainSum = hist->underflow - hist->overflow;
    hist->vertScale += autoGainSum * hist->autoScaleFact;
    if(hist->vertScale <= 0)
      hist->vertScale = 1;
#ifdef DEBUG_AG
    printf("OE: %d\tUE: %d\tS: %.0f\t", hist->overflow, hist->underflow, hist->vertScale);
#endif

    hist->overflow = 0;
    hist->underflow = 0;
  }

  rectH = hist->vertScale * value;

  // Auto scaling logic
  if (rectH > getWinH())
    hist->overflow++;
  if (rectH < 1)
    hist->underflow++;

  //  rectW = floorf((float)getWinW() * (100.0f / (39.0f * hist->fullLen)) * pow(10.0f, (float)-index / (hist->fullLen)));
  //  float sumW = 1.0f///;
  rectW = floorf(getWinW() * (1.0f - pow(10, -1.0f / hist->fullLen))) * pow(10, -(index / hist->fullLen));

  hist->rects[index].x = hist->sumRectW;
  hist->rects[index].y = getWinH() - rectH;
  hist->rects[index].w = rectW;
  hist->rects[index].h = rectH;

  hist->sumRectW += rectW * 1.1;
}

void drawHistogram(Histogram_t* hist) {
  SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
  SDL_RenderClear(rend);

  // Set draw color to green
  SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);

  // Draw multiple filled rectangles at once
  SDL_RenderFillRects(rend, hist->rects, hist->fullLen);

  // Present the renderer
  SDL_RenderPresent(rend);
}

void freeHistogram(Histogram_t* hist) {
  free(hist->rects);
}

uint8_t checkQuiteEvent() {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return 1;
      printf("Exiting\n");
    }
  }
  return 0;
}
