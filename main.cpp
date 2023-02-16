#include <iostream>
#include <vector>
#include <set>

using namespace std;

const int HEIGHT = 3;
const int WIDTH = 6;

enum class State{EMPTY, PLAYER, BOX};
enum class Direction {UP, DOWN, LEFT, RIGHT, SIZE, UNKNOWN};

struct Point {
    int row;
    int col;

    static constexpr int COL_MOVE[static_cast<size_t>(Direction::SIZE)] = {0, 0, -1, 1};
    static constexpr int ROW_MOVE[static_cast<size_t>(Direction::SIZE)] = {-1, 1, 0, 0};

    Point Next(Direction d) const { return {row + ROW_MOVE[static_cast<int>(d)], col + COL_MOVE[static_cast<int>(d)]}; }

    bool IsInBoard() const {
        return row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH;
    }

    bool operator<(const Point& rhs) const {
        return row == rhs.row ? (col < rhs.col) : (row < rhs.row);
    }
};

constexpr int Point::COL_MOVE[4];
constexpr int Point::ROW_MOVE[4];

class Game{
public:
    Game(set<Point>&& boxes, set<Point>&& dests, Point player);
    
    State GetState(const Point& p) const { return m_board[p.row][p.col]; }
    bool IsDest(const Point& p) const { return m_dest[p.row][p.col]; }
    bool IsEnd() const { return m_arrived_count == m_dest_count; }

    Direction GetInput() const;
    bool IsValidMove(Direction d) const;    
    void Show() const;

    void SetState(const Point& p, State s) { m_board[p.row][p.col] = s; }
    void MoveTo(Direction d);
    void Update(Direction d);

private:
    Point m_player;
    const int m_dest_count;
    int m_arrived_count = 0;

    State m_board[HEIGHT][WIDTH] = {{State::EMPTY}};
    bool m_dest[HEIGHT][WIDTH] = {{false}};
};

int main()
{
              // boxes        destinations    player
    Game game({{1,1}, {1,2}}, {{0,1}, {0,2}}, {0,3});
    game.Show();

    while(true) {
        auto input = game.GetInput();
        if(input == Direction::UNKNOWN)
            break;

        game.Update(input);
        game.Show();

        if(game.IsEnd()) {
            cout << "You Win!!\n";
            break;
        }
    }
}

Game::Game(set<Point>&& boxes, set<Point>&& dests, Point player) : m_player(player), m_dest_count(dests.size()) {
    for(const auto& box : boxes)
        SetState(box, State::BOX);

    for(const auto& dest : dests)
    {
        m_dest[dest.row][dest.col] = true;

        if(GetState(dest) == State::BOX)
            ++m_arrived_count;
    }

    SetState(m_player, State::PLAYER);
}

Direction Game::GetInput() const {
    char c;
    
    do {
        cout << "Please enter your move(w/s/a/d/q): ";
        cin >> c;
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
        cout << "Invalid move\n";
}

bool Game::IsValidMove(Direction d) const {
    // 1. Shall not out of board
    auto next = m_player.Next(d);

    if(next.IsInBoard() == false)
        return false;

    // 2. Is Empty
    if(GetState(next) == State::EMPTY)
        return true;

    // 3. Is Box and the same direction has an empty space
    if(GetState(next) == State::BOX)
    {
        auto next_next = next.Next(d);

        if(next_next.IsInBoard() == false)
            return false;

        if(GetState(next_next) == State::EMPTY)
            return true;
    }

    return false;        
}

void Game::Show() const {

    const int WIDTH_WITH_BORDER = WIDTH + 2;

    for(int col = 0 ; col < WIDTH_WITH_BORDER ; ++col)
        cout << '#';
    cout << '\n';

    for(int row = 0 ; row < HEIGHT ; ++row) {
        cout << '#';

        for(int col = 0 ; col < WIDTH ; ++col) {
            Point p{row, col};

            switch(GetState(p)) {
                case State::BOX: cout << (IsDest(p) ? 'O' : 'o'); break;
                case State::PLAYER: cout << (IsDest(p) ? 'P' : 'p'); break;
                default: cout << (IsDest(p) ? '.' : ' ');
            }
        }   

        cout << "#\n"; 
    }        

    for(int col = 0 ; col < WIDTH_WITH_BORDER ; ++col)
        cout << '#';
    cout << '\n';
}