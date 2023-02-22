
#include "game.h"
#include "screen.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <thread>

std::unique_ptr<Game> LoadFromFile(std::string file_name);
Direction GetInput(bool &run);

int main(int argc, char **argv) {
  std::string file_name = argc > 1 ? argv[1] : "stage.txt";

  auto game = LoadFromFile(file_name);

  if (game.get() == nullptr) {
    std::cout << "fail to load stage file " << file_name << "\n";
    return EXIT_FAILURE;
  }

  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();

  try {
    Screen screen((game->GetWidth() + 2) * Screen::GRID_PIXELS,
                  (game->GetHeight() + 2) * Screen::GRID_PIXELS);
    screen.Show(*game);

    auto font = TTF_OpenFont("res/arial.ttf", 72);

    const static int FRAMES = 60;
    bool run = true;

    while (run) {
      if (auto dir = GetInput(run); dir != Direction::UNKNOWN && game->IsEnd() == false)
        game->Update(dir);

      screen.Show(*game);
      std::this_thread::sleep_for(std::chrono::microseconds(1000 / FRAMES));
    }
  } catch (std::string err_msg) {
    std::cout << err_msg;
    return EXIT_FAILURE;
  }

  TTF_Quit();
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