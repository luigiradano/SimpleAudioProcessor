#include <SDL2/SDL.h>

#ifndef HISTOGRAM_SDL
#define HISTOGRAM_SDL
/**
 * @brief Structure representing a histogram and its rendering properties.
 *
 * @details
 * The Histogram_t structure stores both statistical data and rendering
 * parameters for a histogram visualization.
 */
typedef struct {
  float vertScale;     /**< Vertical scaling factor applied to histogram values. */
  float offset;        /**< Offset applied to the values. */
  float autoScaleFact; /**< Automatic scaling factor used for normalizing histogram values. Increase for faster autogain */
  float maxVal;        /**< Maximum value found in the histogram data. */
  float minVal;        /**< Minimum value found in the histogram data. */
  float avgVal;        /**< Average value of the histogram data. */
  int sumRectW;        /**< Summed width of all histogram bars (rectangles) used in rendering. */
  uint16_t underflow;  /**< Count of values that fall below the minimum vertical range. */
  uint16_t overflow;   /**< Count of values that exceed the maximum vertical range. */
  size_t fullLen;      /**< Total number of samples used to build the histogram. */
  SDL_Rect histDims;   /**< Dimensions of the histogram area for rendering. */
  SDL_Rect* rects;     /**< Array of rectangles representing histogram bins for rendering. */
} Histogram_t;
/**
 *   @brief  Setup the histogram and initialize internal variables
 */
void setupHistogram(Histogram_t* hist, size_t fullLen, SDL_Rect size);

/**
 *   @brief  Fill one histogram bin
 */
void fillHistogram(Histogram_t* hist, float value, size_t index);

/**
 *  @brief Draw the histogram on the SDL windwo
 */
void drawHistogram(Histogram_t* hist, SDL_Renderer* rend);

/**
 *   @brief  Fill one histogram bin, the value provided is scaled to the maxValue and the height will be
 *           converted to dBs
 */
void fillHistogramLog(Histogram_t* hist, float value, float maxVal, size_t index);

/**
 * @brief  Free the contents of a histogram
 */
void freeHistogram(Histogram_t* hist);

#endif  // !HISTOGRAM_SDL
