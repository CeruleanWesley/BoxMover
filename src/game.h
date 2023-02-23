#ifndef GAME_H
#define GAME_H

#include <memory>
#include <set>
#include <vector>

enum class State { EMPTY, PLAYER, BOX, EDGE };
enum class Direction { UP, DOWN, LEFT, RIGHT, SIZE, UNKNOWN };

struct Point {
  int row = -1;
  int col = -1;

  static constexpr int COL_MOVE[static_cast<size_t>(Direction::SIZE)] = {0, 0, -1, 1};
  static constexpr int ROW_MOVE[static_cast<size_t>(Direction::SIZE)] = {-1, 1, 0, 0};

  Point Next(Direction d) const {
    return {row + ROW_MOVE[static_cast<int>(d)], col + COL_MOVE[static_cast<int>(d)]};
  }

  bool operator<(const Point& rhs) const {
    return row == rhs.row ? (col < rhs.col) : (row < rhs.row);
  }
};

class Game {
 public:
  Game(int width, int height, const std::set<Point>& boxes, const std::set<Point>& dests,
       const std::set<Point>& edges, Point player);

  int GetWidth() const { return m_width; }
  int GetHeight() const { return m_height; }
  State GetState(const Point& p) const { return m_board[p.row][p.col]; }
  bool IsDest(const Point& p) const { return m_dest[p.row][p.col]; }
  bool IsEnd() const { return m_arrived_count == m_dest_count; }
  bool IsInBoard(const Point& p) const {
    return p.row >= 0 && p.row < m_height && p.col >= 0 && p.col < m_width;
  }

  Direction GetInput() const;
  bool IsValidMove(Direction d) const;
  void Show() const;

  void SetState(const Point& p, State s) { m_board[p.row][p.col] = s; }
  void MoveTo(Direction d);
  void Update(Direction d);

  static std::unique_ptr<Game> StringToGame(const std::string& file_name);

 private:
  int m_width = 8;
  int m_height = 5;

  Point m_player;
  const int m_dest_count;
  int m_arrived_count = 0;

  std::vector<std::vector<State>> m_board;
  std::vector<std::vector<bool>> m_dest;
};

#endif  // GAME_H