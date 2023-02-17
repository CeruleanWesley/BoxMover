#include "game.h"
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(PointTest, IsInBoard) {
  Point p;
  EXPECT_FALSE(p.IsInBoard());

  Point top_left = {0,0};
  EXPECT_TRUE(top_left.IsInBoard());
  EXPECT_FALSE(top_left.Next(Direction::UP).IsInBoard());
  EXPECT_TRUE(top_left.Next(Direction::DOWN).IsInBoard());
  EXPECT_FALSE(top_left.Next(Direction::LEFT).IsInBoard());
  EXPECT_TRUE(top_left.Next(Direction::RIGHT).IsInBoard());

  Point bottom_left = {HEIGHT-1, 0};
  EXPECT_TRUE(bottom_left.IsInBoard());
  EXPECT_TRUE(bottom_left.Next(Direction::UP).IsInBoard());
  EXPECT_FALSE(bottom_left.Next(Direction::DOWN).IsInBoard());
  EXPECT_FALSE(bottom_left.Next(Direction::LEFT).IsInBoard());
  EXPECT_TRUE(bottom_left.Next(Direction::RIGHT).IsInBoard());

  Point top_right = {0, WIDTH-1};
  EXPECT_TRUE(top_right.IsInBoard());
  EXPECT_FALSE(top_right.Next(Direction::UP).IsInBoard());
  EXPECT_TRUE(top_right.Next(Direction::DOWN).IsInBoard());
  EXPECT_TRUE(top_right.Next(Direction::LEFT).IsInBoard());
  EXPECT_FALSE(top_right.Next(Direction::RIGHT).IsInBoard());

  Point bottom_right = {HEIGHT-1, WIDTH-1};
  EXPECT_TRUE(bottom_right.IsInBoard());
  EXPECT_TRUE(bottom_right.Next(Direction::UP).IsInBoard());
  EXPECT_FALSE(bottom_right.Next(Direction::DOWN).IsInBoard());
  EXPECT_TRUE(bottom_right.Next(Direction::LEFT).IsInBoard());
  EXPECT_FALSE(bottom_right.Next(Direction::RIGHT).IsInBoard());
}