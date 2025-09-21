#ifndef SDL_HANDLE
#define SDL_HANDLE

#include <SDL2/SDL.h>
#include <stdint.h>

typedef struct {
  float vertScale;
  float autoScaleFact;
  int sumRectW;
  uint8_t underflow;
  uint8_t overflow;
  size_t fullLen;
  int16_t maxH;
  int16_t  minH;
  SDL_Rect* rects;

} Histogram_t;

void initSDL(uint32_t width, uint32_t height);
SDL_Surface* getSurfSDL();
SDL_Renderer* getRendSDL();
SDL_Window* getWinSDL();
void quitSDL();
uint32_t getWinW();
uint32_t getWinH();


void setupHistogram(Histogram_t* hist, size_t fullLen);
void fillHistogram(Histogram_t* hist, float value, size_t index);
void drawHistogram(Histogram_t* hist);
void fillHistogramDb(Histogram_t* hist, float value, float maxValue, size_t index);
void freeHistogram(Histogram_t* hist);

uint8_t checkQuiteEvent();

#endif
