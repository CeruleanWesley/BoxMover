#include "game.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include <string>

std::unique_ptr<Game> LoadFromFile(std::string file_name);

int main(int argc, char **argv) {
  std::string file_name{"stage.txt"};

  if (argc > 1) file_name = std::string(argv[1]);

  auto game = LoadFromFile(file_name);
  if (game.get() == nullptr) {
    std::cout << "fail to load stage file " << file_name << "\n";
    return 1;
  }

  game->Show();

  while (true) {
    auto input = game->GetInput();
    if (input == Direction::UNKNOWN) break;

    game->Update(input);
    game->Show();

    if (game->IsEnd()) {
      std::cout << "You Win!!\n";
      break;
    }
  }
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