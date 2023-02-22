#include "screen.h"

#include <string>

Screen::Screen(int width, int height) : m_width(width), m_height(height) {
  SDL_CreateWindowAndRenderer(width, height, 0, &m_window, &m_renderer);

  if (nullptr == m_window)
    throw(std::string{"Could not create window: "} + SDL_GetError());

  SDL_SetWindowTitle(m_window, "Box Mover (press ESC to exit)");

  m_font = TTF_OpenFont("res/arial.ttf", 72);
  m_textSurface = TTF_RenderText_Solid(m_font, "You Win!!", {0, 0, 255, 255});
  m_text = SDL_CreateTextureFromSurface(m_renderer, m_textSurface);

  m_matetial = SDL_LoadBMP("res/material.bmp");
  m_texture = SDL_CreateTextureFromSurface(m_renderer, m_matetial);
}

Screen::~Screen() {
  SDL_FreeSurface(m_textSurface);
  SDL_DestroyTexture(m_text);

  SDL_FreeSurface(m_matetial);
  SDL_DestroyTexture(m_texture);

  SDL_DestroyWindow(m_window);
}

void Screen::DrawEndString() {
  SDL_Rect str_position{(m_width - m_textSurface->w) / 2,
                        (m_height - m_textSurface->h) / 2, m_textSurface->w,
                        m_textSurface->h};
  SDL_RenderCopy(m_renderer, m_text, nullptr, &str_position);
}

void Screen::Show(const Game &game) {
  SDL_RenderClear(m_renderer);

  auto GRID_SIZE = Screen::GRID_PIXELS;

  enum class GridType {
    BOX,
    DEST_BOX,
    EDGE,
    SIDE_EDGE,
    DEST,
    PLAYER,
    DEST_PLAYER,
    COUNT
  };
  std::vector<SDL_Rect> src_rects;

  for (int i = 0; i < static_cast<int>(GridType::COUNT); ++i)
    src_rects.push_back({GRID_SIZE * i, 0, GRID_SIZE, GRID_SIZE});

  for (int i = 0; i < game.GetWidth() + 2; ++i) {
    SDL_Rect dest_rect{GRID_SIZE * i, 0, GRID_SIZE, GRID_SIZE};
    SDL_RenderCopy(m_renderer, m_texture, &src_rects[static_cast<int>(GridType::EDGE)],
                   &dest_rect);
  }

  for (int row = 0; row < game.GetHeight(); ++row) {
    SDL_Rect dest_rect{0, GRID_SIZE * (row + 1), GRID_SIZE, GRID_SIZE};
    SDL_RenderCopy(m_renderer, m_texture,
                   &src_rects[static_cast<int>(GridType::SIDE_EDGE)], &dest_rect);

    for (int col = 0; col < game.GetWidth(); ++col) {
      Point p{row, col};
      SDL_Rect src_rect{0, 0, GRID_SIZE, GRID_SIZE};
      dest_rect =
          SDL_Rect{GRID_SIZE * (col + 1), GRID_SIZE * (row + 1), GRID_SIZE, GRID_SIZE};
      switch (game.GetState(p)) {
        case State::BOX:
          src_rect = game.IsDest(p) ? src_rects[static_cast<int>(GridType::DEST_BOX)]
                                    : src_rects[static_cast<int>(GridType::BOX)];

          SDL_RenderCopy(m_renderer, m_texture, &src_rect, &dest_rect);
          break;

        case State::PLAYER:
          src_rect = game.IsDest(p) ? src_rects[static_cast<int>(GridType::DEST_PLAYER)]
                                    : src_rects[static_cast<int>(GridType::PLAYER)];

          SDL_RenderCopy(m_renderer, m_texture, &src_rect, &dest_rect);
          break;

        default:
          if (game.IsDest(p))
            SDL_RenderCopy(m_renderer, m_texture,
                           &src_rects[static_cast<int>(GridType::DEST)], &dest_rect);
      }
    }

    dest_rect = SDL_Rect{GRID_SIZE * (game.GetWidth() + 1), GRID_SIZE * (row + 1),
                         GRID_SIZE, GRID_SIZE};
    SDL_RenderCopy(m_renderer, m_texture,
                   &src_rects[static_cast<int>(GridType::SIDE_EDGE)], &dest_rect);
  }

  for (int i = 0; i < game.GetWidth() + 2; ++i) {
    SDL_Rect dest_rect{GRID_SIZE * i, GRID_SIZE * (game.GetHeight() + 1), GRID_SIZE,
                       GRID_SIZE};
    SDL_RenderCopy(m_renderer, m_texture, &src_rects[static_cast<int>(GridType::EDGE)],
                   &dest_rect);
  }

  if (game.IsEnd()) DrawEndString();

  SDL_RenderPresent(m_renderer);
}
