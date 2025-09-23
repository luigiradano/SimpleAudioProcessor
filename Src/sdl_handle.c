#include "../Inc/sdl_handle.h"

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
uint8_t checkQuitEvent() {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return 1;
      printf("Exiting\n");
    }
  }
  return 0;
}
