#include "game.h"
#include <iostream>

constexpr int Point::COL_MOVE[4];
constexpr int Point::ROW_MOVE[4];

Game::Game(int width, int height, std::set<Point>&& boxes, std::set<Point>&& dests, Point player) 
: m_width(width), 
  m_height(height), 
  m_player(player), 
  m_dest_count(dests.size()), 
  m_board(height, std::vector<State>(width, State::EMPTY)),
  m_dest(height, std::vector<bool>(width, false)) {
    for(const auto& box : boxes)
        SetState(box, State::BOX);

    for(const auto& dest : dests)
    {
        m_dest[dest.row][dest.col] = true;

        if(GetState(dest) == State::BOX)
            ++m_arrived_count;
    }

    if(IsInBoard(m_player) == false)
      m_player = {0, 0};

    SetState(m_player, State::PLAYER);
}

Direction Game::GetInput() const {
    char c;
    
    do {
        std::cout << "Please enter your move(w/s/a/d/q): ";
        std::cin >> c;
    } while(c != 'w' && c != 'a' && c != 's' && c != 'd' && c != 'q');

    switch(c){
        case 'w': return Direction::UP;
        case 's': return Direction::DOWN;
        case 'a': return Direction::LEFT;
        case 'd': return Direction::RIGHT;
    }

    return Direction::UNKNOWN;
}

void Game::MoveTo(Direction d) {
    auto next = m_player.Next(d);
    
    if(GetState(next) == State::BOX)
    {
        auto next_next = next.Next(d);

        SetState(next_next, State::BOX);

        if(IsDest(next) != IsDest(next_next))
            m_arrived_count += IsDest(next_next) ? 1 : -1;
    }

    SetState(next, State::PLAYER);
    SetState(m_player, State::EMPTY);    

    m_player = next;
}

void Game::Update(Direction d) {
    if(IsValidMove(d))
        MoveTo(d);        
    else
        std::cout << "Invalid move\n";
}

bool Game::IsValidMove(Direction d) const {
    // 1. Shall not out of board
    auto next = m_player.Next(d);

    if(IsInBoard(next) == false)
        return false;

    // 2. Is Empty
    if(GetState(next) == State::EMPTY)
        return true;

    // 3. Is Box and the same direction has an empty space
    if(GetState(next) == State::BOX)
    {
        auto next_next = next.Next(d);

        if(IsInBoard(next_next) == false)
            return false;

        if(GetState(next_next) == State::EMPTY)
            return true;
    }

    return false;        
}

void Game::Show() const {

    const int WIDTH_WITH_BORDER = m_width + 2;

    for(int col = 0 ; col < WIDTH_WITH_BORDER ; ++col)
        std::cout << '#';
    std::cout << '\n';

    for(int row = 0 ; row < m_height ; ++row) {
        std::cout << '#';

        for(int col = 0 ; col < m_width ; ++col) {
            Point p{row, col};

            switch(GetState(p)) {
                case State::BOX: std::cout << (IsDest(p) ? 'O' : 'o'); break;
                case State::PLAYER: std::cout << (IsDest(p) ? 'P' : 'p'); break;
                default: std::cout << (IsDest(p) ? '.' : ' ');
            }
        }   

        std::cout << "#\n"; 
    }        

    for(int col = 0 ; col < WIDTH_WITH_BORDER ; ++col)
        std::cout << '#';
    std::cout << '\n';
}