#ifndef SCREEN_H
#define SCREEN_H

#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Screen {
 public:
  static const int GRID_PIXELS = 37;  // depend on material

  Screen(const Game &game);
  ~Screen();

  void Show();

 private:
  void DrawEndString();

 private:
  const Game &m_game;
  int m_width = 800;
  int m_height = 600;

  SDL_Window *m_window = nullptr;
  SDL_Renderer *m_renderer = nullptr;
  SDL_Surface *m_matetial = nullptr;
  SDL_Texture *m_texture = nullptr;

  TTF_Font *m_font = nullptr;
  SDL_Surface *m_textSurface = nullptr;
  SDL_Texture *m_text = nullptr;
};

#endif  // SCREEN_H