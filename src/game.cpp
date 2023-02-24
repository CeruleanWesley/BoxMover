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
  for (const auto& box : boxes)
    if (IsInBoard(box)) SetState(box, State::BOX);

  for (const auto& edge : edges)
    if (IsInBoard(edge)) SetState(edge, State::EDGE);

  for (const auto& dest : dests) {
    if (IsInBoard(dest)) {
      m_dest[dest.row][dest.col] = true;
      if (GetState(dest) == State::BOX) ++m_arrived_count;
    }
  }

  if (IsInBoard(m_player) == false) m_player = {0, 0};

  SetState(m_player, State::PLAYER);
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

bool Game::Update(Direction d) {
  if (IsValidMove(d)) {
    MoveTo(d);
    return true;
  }

  return false;
}

bool Game::IsValidMove(Direction d) const {
  auto next = m_player.Next(d);

  if (IsInBoard(next) == false || GetState(next) == State::EDGE) return false;

  if (GetState(next) == State::EMPTY) return true;

  if (GetState(next) == State::BOX) {
    auto next_next = next.Next(d);

    if (IsInBoard(next_next) == false || GetState(next_next) == State::EDGE) return false;

    if (GetState(next_next) == State::EMPTY) return true;
  }

  return false;
}

std::string Game::ToString() const {
  std::stringstream ss;

  for (int row = 0; row < m_height; ++row) {
    for (int col = 0; col < m_width; ++col) {
      Point p{row, col};

      switch (GetState(p)) {
        case State::BOX:
          ss << (IsDest(p) ? 'O' : 'o');
          break;
        case State::PLAYER:
          ss << (IsDest(p) ? 'P' : 'p');
          break;
        case State::EDGE:
          ss << '#';
          break;
        default:
          ss << (IsDest(p) ? '.' : ' ');
      }
    }
    ss << "\n";
  }

  return ss.str();
}

std::unique_ptr<Game> Game::CreateGameByString(const std::string& stage) {
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
