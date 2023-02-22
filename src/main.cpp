
#include "game.h"

#include <SDL2/SDL.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <thread>

void FillRect(SDL_Renderer *renderer, const SDL_Rect &rect, const SDL_Color &color);
void ShowBoard(SDL_Renderer *renderer, const Game &game);
Direction GetInput(bool &run);
std::unique_ptr<Game> LoadFromFile(std::string file_name);

const int WIDTH = 800, HEIGHT = 600;

int main(int argc, char **argv) {
  std::string file_name{"stage.txt"};

  if (argc > 1) file_name = std::string(argv[1]);

  auto game = LoadFromFile(file_name);
  if (game.get() == nullptr) {
    std::cout << "fail to load stage file " << file_name << "\n";
    return 1;
  }

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

  if (nullptr == window) {
    std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  int frames = 60;

  ShowBoard(renderer, *game);

  bool run = true;
  while (run) {
    if (auto dir = GetInput(run); dir != Direction::UNKNOWN) {
      game->Update(dir);
    }

    ShowBoard(renderer, *game);

    if (game->IsEnd()) {
      std::cout << "You Win!!\n";
      break;
    }

    std::this_thread::sleep_for(std::chrono::microseconds(1000 / frames));
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}

std::unique_ptr<Game> LoadFromFile(std::string file_name) {
  std::set<Point> boxes;
  std::set<Point> dests;
  Point player;

  int width = 0, height = 0;

  std::string line;

  std::ifstream inf(file_name.c_str());

  if (inf.is_open() == false) return nullptr;

  while (std::getline(inf, line)) {
    if (height == 0) {
      if (line.find_first_not_of('#') != std::string::npos || line.size() <= 2)
        return nullptr;

      width = line.size() - 2;
    } else {
      // end line of the stage
      if (line.find_first_not_of('#') == std::string::npos)
        return std::make_unique<Game>(width, height - 1, std::move(boxes),
                                      std::move(dests), player);

      if (line.size() != width + 2 || line[0] != '#' || line[line.size() - 1] != '#')
        return nullptr;

      for (int i = 1; i < line.size() - 1; ++i) {
        switch (line[i]) {
          case '#':
            return nullptr;

          case 'P':
            dests.insert({height - 1, i - 1});
          case 'p':
            player = {height - 1, i - 1};
            break;

          case 'O':
            dests.insert({height - 1, i - 1});
          case 'o':
            boxes.insert({height - 1, i - 1});
            break;

          case '.':
            dests.insert({height - 1, i - 1});
            break;
        }
      }
    }

    ++height;
  }

  return nullptr;
}

void FillRect(SDL_Renderer *renderer, const SDL_Rect &rect, const SDL_Color &color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(renderer, &rect);
}

void ShowBoard(SDL_Renderer *renderer, const Game &game) {
  SDL_RenderClear(renderer);

  const int GRID_SIZE = 16;

  for (int i = 0; i < game.GetWidth() + 2; ++i) {
    FillRect(renderer, {GRID_SIZE * i, 0, GRID_SIZE, GRID_SIZE}, {255, 255, 255, 255});
  }

  for (int row = 0; row < game.GetHeight(); ++row) {
    FillRect(renderer, {0, GRID_SIZE * (row + 1), GRID_SIZE, GRID_SIZE},
             {255, 255, 255, 255});

    for (int col = 0; col < game.GetWidth(); ++col) {
      Point p{row, col};
      SDL_Color color{0, 0, 0, 255};

      switch (game.GetState(p)) {
        case State::BOX:
          color =
              game.IsDest(p) ? SDL_Color{255, 0, 0, 255} : SDL_Color{255, 0, 255, 255};
          break;

        case State::PLAYER:
          color =
              game.IsDest(p) ? SDL_Color{0, 255, 0, 255} : SDL_Color{0, 255, 255, 255};
          break;

        default:
          if (game.IsDest(p)) color = SDL_Color{0, 0, 255, 255};
      }

      FillRect(renderer,
               {GRID_SIZE * (col + 1), GRID_SIZE * (row + 1), GRID_SIZE, GRID_SIZE},
               color);
    }

    FillRect(
        renderer,
        {GRID_SIZE * (game.GetWidth() + 1), GRID_SIZE * (row + 1), GRID_SIZE, GRID_SIZE},
        {255, 255, 255, 255});
  }

  for (int i = 0; i < game.GetWidth() + 2; ++i) {
    FillRect(renderer,
             {GRID_SIZE * i, GRID_SIZE * (game.GetHeight() + 1), GRID_SIZE, GRID_SIZE},
             {255, 255, 255, 255});
  }

  SDL_RenderPresent(renderer);
}

Direction GetInput(bool &run) {
  run = true;
  SDL_Event windowEvent;

  if (SDL_PollEvent(&windowEvent)) {
    switch (windowEvent.type) {
      case SDL_QUIT:
        run = false;
        break;
      case SDL_KEYDOWN:
        switch (windowEvent.key.keysym.sym) {
          case SDLK_ESCAPE:
            run = false;
            break;
          case SDLK_UP:
            return Direction::UP;
          case SDLK_DOWN:
            return Direction::DOWN;
          case SDLK_LEFT:
            return Direction::LEFT;

          case SDLK_RIGHT:
            return Direction::RIGHT;
        }
    }
  }

  return Direction::UNKNOWN;
}