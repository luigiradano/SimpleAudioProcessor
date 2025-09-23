#include "../Inc/histogram_sdl.h"

void setupHistogram(Histogram_t* hist, size_t fullLen, SDL_Rect size) {
  hist->vertScale = 20;
  hist->autoScaleFact = 1;
  hist->sumRectW = 0;
  hist->underflow = 0;
  hist->overflow = 0;
  hist->maxVal = 0;
  hist->minVal = 0;
  hist->avgVal = 0;
  hist->histDims = size;
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

  if (!isinf(value) && value > hist->maxVal)
    hist->maxVal = value;
  if (!isinf(value) && value < hist->minVal)
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
  if (rectH > hist->histDims.h){
    hist->overflow++;
    rectH = hist->histDims.h;
  }
  if (rectH < 1){
    hist->underflow++;
    rectH = 1;
  }

  rectW = floorf(hist->histDims.w * (1.0f - pow(10, -1.0f / hist->fullLen))) * pow(10, -((float) index / hist->fullLen));

  if(rectW + hist->sumRectW <= hist->histDims.w){
  hist->rects[index].x = hist->histDims.x + hist->sumRectW;
  hist->rects[index].y = (hist->histDims.y + hist->histDims.h) - rectH;
  hist->rects[index].w = rectW;
  hist->rects[index].h = rectH;
  }

  hist->sumRectW += rectW * 1.1;
}

void drawHistogram(Histogram_t* hist, SDL_Renderer* rend) {
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


