#include <iostream>

using namespace std;

const int BORDER_SIZE = 1;
const int ROWS = 3;
const int COLS = 6;
const int TOTAL_ROWS = ROWS + BORDER_SIZE*2;
const int TOTAL_COLS = COLS + BORDER_SIZE*2;

const int EMPTY = 0;
const int PLAYER = 1;
const int BOX = 2;
const int DEST = 3;
const int BORDER = 4;

void ShowBoard(int board[TOTAL_ROWS][TOTAL_COLS])
{
    for(int row = 0 ; row < TOTAL_ROWS ; ++row) {
        for(int col = 0 ; col < TOTAL_COLS ; ++col) {
            switch(board[row][col]) {
                case BORDER: cout << '#'; break;
                case DEST: cout << '.'; break;
                case BOX: cout << 'o'; break;
                case PLAYER: cout << 'p'; break;
                default: cout << ' ';
            }
        }   
        cout << '\n';   
    }
}

int main()
{
    int board[TOTAL_ROWS][TOTAL_COLS] = {{0}};

    // assign border
    for(int col = 0 ; col < TOTAL_COLS ; ++col) {
        board[0][col] = board[TOTAL_ROWS-1][col] = BORDER;
    }

    for(int row = 0 ; row < TOTAL_COLS ; ++row) {
        board[row][0] = board[row][TOTAL_COLS-1] = BORDER;
    }

    board[1][2] = board[1][3] = DEST;
    board[2][2] = board[2][3] = BOX;
    board[1][4] = PLAYER;

    ShowBoard(board);
}