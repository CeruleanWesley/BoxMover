#include "game.h"
#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include <vector>

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

TEST(GameTest, PlayGame1) {
  Game game({{1,1}, {1,2}}, {{0,1}, {0,2}}, {0,3});
  EXPECT_FALSE(game.IsEnd());
    
  testing::internal::CaptureStdout();
  game.Show();
  std::string output = testing::internal::GetCapturedStdout();  
  EXPECT_STREQ(output.c_str(), 1 + R"(
########
# ..p  #
# oo   #
#      #
########
)");
  
  game.Update(Direction::DOWN);
  EXPECT_EQ(game.GetState({1,3}), State::PLAYER);
  EXPECT_EQ(game.GetState({0,3}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());
    
  testing::internal::CaptureStdout();
  game.Show();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(), 1 + R"(
########
# ..   #
# oop  #
#      #
########
)");  
  
  game.Update(Direction::DOWN);
  EXPECT_EQ(game.GetState({2,3}), State::PLAYER);
  EXPECT_EQ(game.GetState({1,3}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());
  
  testing::internal::CaptureStdout();
  game.Show();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(), 1 + R"(
########
# ..   #
# oo   #
#   p  #
########
)");  

  game.Update(Direction::LEFT);
  EXPECT_EQ(game.GetState({2,2}), State::PLAYER);
  EXPECT_EQ(game.GetState({2,3}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());
    
  testing::internal::CaptureStdout();
  game.Show();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(), 1 + R"(
########
# ..   #
# oo   #
#  p   #
########
)");  

  game.Update(Direction::UP);
  EXPECT_EQ(game.GetState({0,2}), State::BOX);
  EXPECT_EQ(game.GetState({1,2}), State::PLAYER);
  EXPECT_EQ(game.GetState({2,2}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());
  
  testing::internal::CaptureStdout();
  game.Show();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(), 1 + R"(
########
# .O   #
# op   #
#      #
########
)");  

  game.Update(Direction::DOWN);
  EXPECT_EQ(game.GetState({2,2}), State::PLAYER);
  EXPECT_EQ(game.GetState({1,2}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());
  
  testing::internal::CaptureStdout();
  game.Show();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(), 1 + R"(
########
# .O   #
# o    #
#  p   #
########
)");  

  game.Update(Direction::LEFT);
  EXPECT_EQ(game.GetState({2,1}), State::PLAYER);
  EXPECT_EQ(game.GetState({2,2}), State::EMPTY);
  EXPECT_FALSE(game.IsEnd());
  
  testing::internal::CaptureStdout();
  game.Show();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(), 1 + R"(
########
# .O   #
# o    #
# p    #
########
)");  

  game.Update(Direction::UP);
  EXPECT_EQ(game.GetState({0,1}), State::BOX);  
  EXPECT_EQ(game.GetState({1,1}), State::PLAYER);
  EXPECT_EQ(game.GetState({2,1}), State::EMPTY);
  EXPECT_TRUE(game.IsEnd());
  
  testing::internal::CaptureStdout();
  game.Show();
  EXPECT_STREQ(testing::internal::GetCapturedStdout().c_str(), 1 + R"(
########
# OO   #
# p    #
#      #
########
)");  
}

TEST(GameTest, PlayGame2) {
  Game game({{0,1}, {0,2}}, {{0,1}, {0,2}}, {0,3});
  EXPECT_TRUE(game.IsEnd());
    
  testing::internal::CaptureStdout();
  game.Show();
  std::string output = testing::internal::GetCapturedStdout();  
  EXPECT_STREQ(output.c_str(), 1 + R"(
########
# OOp  #
#      #
#      #
########
)");
}

TEST(GameTest, InputTest) {
	
  std::ofstream outf ("test.txt");
  outf << "s s a w s a d q" << std::endl;
  outf.close();

  std::ifstream inf("test.txt");
  std::streambuf *inbuf = std::cin.rdbuf(inf.rdbuf());
 
  Game game({{1,1}, {1,2}}, {{0,1}, {0,2}}, {0,3}); 
  
  std::vector<std::pair<Direction,bool>> result{
    {Direction::DOWN, true}, 
    {Direction::DOWN, true},
    {Direction::LEFT, true},
    {Direction::UP, true},
    {Direction::DOWN, true},
    {Direction::LEFT, true},
    {Direction::RIGHT, true},
    {Direction::UNKNOWN, false}};
  
  for(auto r : result) {
    auto d = game.GetInput();
    EXPECT_EQ(d, r.first);
    EXPECT_EQ(game.IsValidMove(d), r.second);
    
    if(game.IsValidMove(d))
    	game.Update(d);
  }
    
  std::cin.rdbuf(inbuf);
  inf.close();
}
