#include <SDL2/SDL.h>

#ifndef HISTOGRAM_SDL
#define HISTOGRAM_SDL


void initSDL(uint32_t width, uint32_t height);
SDL_Surface* getSurfSDL();
SDL_Renderer* getRendSDL();
SDL_Window* getWinSDL();
void quitSDL();
uint32_t getWinW();
uint32_t getWinH();



#endif // !HISTOGRAM_SDL


