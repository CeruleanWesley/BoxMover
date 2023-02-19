#include "game.h"
#include <iostream>

int main()
{
              // boxes        destinations    player
    Game game(6, 3, {{1,1}, {1,2}}, {{0,1}, {0,2}}, {0,3});
    game.Show();

    while(true) {
        auto input = game.GetInput();
        if(input == Direction::UNKNOWN)
            break;

        game.Update(input);
        game.Show();

        if(game.IsEnd()) {
            std::cout << "You Win!!\n";
            break;
        }
    }
}
