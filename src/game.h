#include <set>

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

class Game{
public:
    Game(std::set<Point>&& boxes, std::set<Point>&& dests, Point player);
    
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