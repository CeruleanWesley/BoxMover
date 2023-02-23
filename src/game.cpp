#include "game.h"

#include <cmath>
#include <iostream>
#include <sstream>

constexpr int Point::COL_MOVE[4];
constexpr int Point::ROW_MOVE[4];

Game::Game(int width, int height, const std::set<Point>& boxes,
           const std::set<Point>& dests, const std::set<Point>& edges, Point player)
    : m_width(width),
      m_height(height),
      m_player(player),
      m_dest_count(dests.size()),
      m_board(height, std::vector<State>(width, State::EMPTY)),
      m_dest(height, std::vector<bool>(width, false)) {
  for (const auto& box : boxes) SetState(box, State::BOX);

  for (const auto& edge : edges) SetState(edge, State::EDGE);

  for (const auto& dest : dests) {
    m_dest[dest.row][dest.col] = true;

    if (GetState(dest) == State::BOX) ++m_arrived_count;
  }

  if (IsInBoard(m_player) == false) m_player = {0, 0};

  SetState(m_player, State::PLAYER);
}

Direction Game::GetInput() const {
  char c;

  do {
    std::cout << "Please enter your move(w/s/a/d/q): ";
    std::cin >> c;
  } while (c != 'w' && c != 'a' && c != 's' && c != 'd' && c != 'q');

  switch (c) {
    case 'w':
      return Direction::UP;
    case 's':
      return Direction::DOWN;
    case 'a':
      return Direction::LEFT;
    case 'd':
      return Direction::RIGHT;
  }

  return Direction::UNKNOWN;
}

void Game::MoveTo(Direction d) {
  auto next = m_player.Next(d);

  if (GetState(next) == State::BOX) {
    auto next_next = next.Next(d);

    SetState(next_next, State::BOX);

    if (IsDest(next) != IsDest(next_next)) m_arrived_count += IsDest(next_next) ? 1 : -1;
  }

  SetState(next, State::PLAYER);
  SetState(m_player, State::EMPTY);

  m_player = next;
}

void Game::Update(Direction d) {
  if (IsValidMove(d))
    MoveTo(d);
  else
    std::cout << "Invalid move\n";
}

bool Game::IsValidMove(Direction d) const {
  // 1. Shall not out of board
  auto next = m_player.Next(d);

  if (IsInBoard(next) == false || GetState(next) == State::EDGE) return false;

  // 2. Is Empty
  if (GetState(next) == State::EMPTY) return true;

  // 3. Is Box and the same direction has an empty space
  if (GetState(next) == State::BOX) {
    auto next_next = next.Next(d);

    if (IsInBoard(next_next) == false || GetState(next_next) == State::EDGE) return false;

    if (GetState(next_next) == State::EMPTY) return true;
  }

  return false;
}

void Game::Show() const {
  const int WIDTH_WITH_BORDER = m_width + 2;

  for (int row = 0; row < m_height; ++row) {
    for (int col = 0; col < m_width; ++col) {
      Point p{row, col};

      switch (GetState(p)) {
        case State::BOX:
          std::cout << (IsDest(p) ? 'O' : 'o');
          break;
        case State::PLAYER:
          std::cout << (IsDest(p) ? 'P' : 'p');
          break;
        case State::EDGE:
          std::cout << '#';
          break;
        default:
          std::cout << (IsDest(p) ? '.' : ' ');
      }
    }
    std::cout << "\n";
  }
}

std::unique_ptr<Game> Game::StringToGame(const std::string& stage) {
  std::set<Point> boxes;
  std::set<Point> dests;
  std::set<Point> edges;
  Point player;

  int width = 0, height = 0;

  std::stringstream ss(stage);
  std::string line;

  while (std::getline(ss, line)) {
    width = std::max(width, static_cast<int>(line.size()));
    for (int i = 0; i < line.size(); ++i) {
      Point p{height, i};

      switch (line[i]) {
        case '#':
          edges.insert(p);
          break;

        case 'P':
          dests.insert(p);
        case 'p':
          player = p;
          break;

        case 'O':
          dests.insert(p);
        case 'o':
          boxes.insert(p);
          break;

        case '.':
          dests.insert(p);
          break;
      }
    }

    ++height;
  }

  return std::make_unique<Game>(width, height, boxes, dests, edges, player);
}
