#include "game.h"

#include <gtest/gtest.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <vector>

// Demonstrate some basic assertions.
TEST(GameTest, IsInBoard) {
  int width = 8, height = 5;
  Game game{width, height, {}, {}, {}, {2, 2}};

  Point p;
  EXPECT_FALSE(game.IsInBoard(p));

  Point top_left = {0, 0};
  EXPECT_TRUE(game.IsInBoard(top_left));
  EXPECT_FALSE(game.IsInBoard(top_left.Next(Direction::UP)));
  EXPECT_TRUE(game.IsInBoard(top_left.Next(Direction::DOWN)));
  EXPECT_FALSE(game.IsInBoard(top_left.Next(Direction::LEFT)));
  EXPECT_TRUE(game.IsInBoard(top_left.Next(Direction::RIGHT)));

  Point bottom_left = {height - 1, 0};
  EXPECT_TRUE(game.IsInBoard(bottom_left));
  EXPECT_TRUE(game.IsInBoard(bottom_left.Next(Direction::UP)));
  EXPECT_FALSE(game.IsInBoard(bottom_left.Next(Direction::DOWN)));
  EXPECT_FALSE(game.IsInBoard(bottom_left.Next(Direction::LEFT)));
  EXPECT_TRUE(game.IsInBoard(bottom_left.Next(Direction::RIGHT)));

  Point top_right = {0, width - 1};
  EXPECT_TRUE(game.IsInBoard(top_right));
  EXPECT_FALSE(game.IsInBoard(top_right.Next(Direction::UP)));
  EXPECT_TRUE(game.IsInBoard(top_right.Next(Direction::DOWN)));
  EXPECT_TRUE(game.IsInBoard(top_right.Next(Direction::LEFT)));
  EXPECT_FALSE(game.IsInBoard(top_right.Next(Direction::RIGHT)));

  Point bottom_right = {height - 1, width - 1};
  EXPECT_TRUE(game.IsInBoard(bottom_right));
  EXPECT_TRUE(game.IsInBoard(bottom_right.Next(Direction::UP)));
  EXPECT_FALSE(game.IsInBoard(bottom_right.Next(Direction::DOWN)));
  EXPECT_TRUE(game.IsInBoard(bottom_right.Next(Direction::LEFT)));
  EXPECT_FALSE(game.IsInBoard(bottom_right.Next(Direction::RIGHT)));
}

TEST(GameTest, PlayGame1) {
  auto game_ptr = Game::CreateGameByString(1 + R"(
########
# ..p  #
# oo   #
#      #
########
)");

  auto& game = *game_ptr;

  EXPECT_FALSE(game.IsEnd());

  EXPECT_STREQ(game.ToString().c_str(), 1 + R"(
########
# ..p  #
# oo   #
#      #
########
)");

  game.Update(Direction::DOWN);
  EXPECT_EQ(game.GetState({2, 4}), State::PLAYER);
  EXPECT_EQ(game.GetState({1, 4}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());

  EXPECT_STREQ(game.ToString().c_str(), 1 + R"(
########
# ..   #
# oop  #
#      #
########
)");

  game.Update(Direction::DOWN);
  EXPECT_EQ(game.GetState({3, 4}), State::PLAYER);
  EXPECT_EQ(game.GetState({2, 4}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());

  EXPECT_STREQ(game.ToString().c_str(), 1 + R"(
########
# ..   #
# oo   #
#   p  #
########
)");

  game.Update(Direction::LEFT);
  EXPECT_EQ(game.GetState({3, 3}), State::PLAYER);
  EXPECT_EQ(game.GetState({3, 4}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());

  EXPECT_STREQ(game.ToString().c_str(), 1 + R"(
########
# ..   #
# oo   #
#  p   #
########
)");

  game.Update(Direction::UP);
  EXPECT_EQ(game.GetState({1, 3}), State::BOX);
  EXPECT_EQ(game.GetState({2, 3}), State::PLAYER);
  EXPECT_EQ(game.GetState({3, 3}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());

  EXPECT_STREQ(game.ToString().c_str(), 1 + R"(
########
# .O   #
# op   #
#      #
########
)");

  game.Update(Direction::DOWN);
  EXPECT_EQ(game.GetState({3, 3}), State::PLAYER);
  EXPECT_EQ(game.GetState({2, 3}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());

  EXPECT_STREQ(game.ToString().c_str(), 1 + R"(
########
# .O   #
# o    #
#  p   #
########
)");

  game.Update(Direction::LEFT);
  EXPECT_EQ(game.GetState({3, 2}), State::PLAYER);
  EXPECT_EQ(game.GetState({3, 3}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());

  EXPECT_STREQ(game.ToString().c_str(), 1 + R"(
########
# .O   #
# o    #
# p    #
########
)");

  game.Update(Direction::UP);
  EXPECT_EQ(game.GetState({1, 2}), State::BOX);
  EXPECT_EQ(game.GetState({2, 2}), State::PLAYER);
  EXPECT_EQ(game.GetState({3, 2}), State::EMPTY);
  EXPECT_TRUE(game.IsEnd());

  EXPECT_STREQ(game.ToString().c_str(), 1 + R"(
########
# OO   #
# p    #
#      #
########
)");
}

TEST(GameTest, PlayGame2) {
  auto game_ptr = Game::CreateGameByString(1 + R"(
########
# OOp  #
#      #
#      #
########
)");

  auto& game = *game_ptr;

  EXPECT_TRUE(game.IsEnd());

  EXPECT_STREQ(game.ToString().c_str(), 1 + R"(
########
# OOp  #
#      #
#      #
########
)");
}