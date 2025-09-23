#ifndef SDL_HANDLE
#define SDL_HANDLE

#include <SDL2/SDL.h>
#include <stdint.h>



/**
 *  @brief  Initialzie the SDL window
 */
void initSDL(uint32_t width, uint32_t height);

/**
 * @brief Retrieves the main SDL surface associated with the window.
 *
 * @return A pointer to the SDL_Surface, or NULL if unavailable.
 */
SDL_Surface* getSurfSDL();

/**
 * @brief Retrieves the main SDL renderer.
 *
 * @return A pointer to the SDL_Renderer, or NULL if unavailable.
 */
SDL_Renderer* getRendSDL();

/**
 * @brief Retrieves the SDL window handle.
 *
 * @return A pointer to the SDL_Window, or NULL if unavailable.
 */
SDL_Window* getWinSDL();

/**
 * @brief Cleans up and shuts down the SDL subsystems.
 *
 * This function should be called before exiting to properly release
 * resources and close the SDL window and renderer.
 */
void quitSDL();

/**
 * @brief Gets the current width of the SDL window.
 *
 * @return The width of the SDL window in pixels.
 */
uint32_t getWinW();

/**
 * @brief Gets the current height of the SDL window.
 *
 * @return The height of the SDL window in pixels.
 */
uint32_t getWinH();



uint8_t checkQuitEvent();

#endif
