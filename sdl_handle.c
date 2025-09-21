#include "sdl_handle.h"

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
  hist->maxH = 0;
  hist->minH = 0;
  hist->fullLen = fullLen;
  hist->rects = (SDL_Rect*)malloc(sizeof(SDL_Rect) * fullLen);
}

void fillHistogramDb(Histogram_t* hist, float value, float maxValue, size_t index) {
  fillHistogram(hist, 10 * log10(value) / log10(maxValue), index);
}
// returns new vertScale factor
void fillHistogram(Histogram_t* hist, float value, size_t index) {
  int16_t rectH, rectW;

  if (index == 0) {
    hist->sumRectW = 0;

    uint8_t condition = hist->overflow != 0 & hist->underflow != 0 << 1;

    switch (condition) {
      case 0b01:
        hist->vertScale += hist->autoScaleFact;
        break;
      case 0b10:
        hist->vertScale -= hist->autoScaleFact;
        break;
      case 0b11:
        if (hist->overflow > hist->underflow)
          hist->vertScale -= hist->autoScaleFact;
        else
          hist->vertScale += hist->autoScaleFact;
    }

    hist->overflow = 0;
    hist->underflow = 0;
    hist->minH = getWinH();
    hist->maxH = -1;
  }

  rectH = hist->vertScale * value;

  rectW = floorf((float)getWinW() * (100.0f / (39.0f * hist->fullLen)) / pow(10.0f, (float)index / (hist->fullLen))) + 1;

  if (rectH < hist->minH)
    hist->minH = rectH;

  if (rectH > hist->maxH)
    hist->maxH = rectH;

  if (hist->minH < 1)
    hist->underflow++;
  if (hist->maxH > (int16_t)getWinH())
    hist->overflow++;

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
