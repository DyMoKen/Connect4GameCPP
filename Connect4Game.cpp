#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int rows = 1;
int cols = 1;

void draw_board(vector<vector<int>>& board); // draw board on screen
void put_piece(int colNum, vector<vector<int>>& board, int player_num); // drop a piece into column
bool check_board_in_pos(int row, int col, vector<vector<int>>& board, int player_num); // check for winning condition for given position in board
vector<int> transpose_column(vector<vector<int>>& board, int col); // take a column of a board as an array

int main(int argc, char* argv[])
{
    cout << "Enter size of game board (format: rows cols) -> ";
    cin >> cols >> rows;

    vector<vector<int>> board;

    for (int i = 0; i < rows; i++)
    {
        board.emplace_back();
        for (int j = 0; j < cols; j++)
        {
            board[i].push_back(0);
        }
    }

    draw_board(board);

    cout << "Enter the column number where to put a piece -> ";
    int colNum;
    cin >> colNum;

    put_piece(colNum, board, 1);
    draw_board(board);

    return 0;
}

void draw_board(vector<vector<int>>& board)
{
    cout << "  ";
    for (int i = 0; i < cols; i++)
    {
        cout << i + 1 << " ";
    }
    cout << endl;
    
    for (int i = rows - 1; i >= 0; i--)
    {
        cout << "| ";
        for (int j = 0; j < cols; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "|" << endl;
    }
}

void put_piece(int colNum, vector<vector<int>>& board, int player_num)
{
    colNum--;
    int rowNum = 0;

    // find free place in chosen column
    while (rowNum < board.size() && board[rowNum][colNum] != 0)
        rowNum++;

    board[rowNum][colNum] = player_num;
}

bool check_board_in_pos(int row, int col, vector<vector<int>>& board, int player_num)
{
    // idea is to use window of 4 to check sum of elements around given position at board[row][col]
    // Every player has own unique numbers.
    // To get 4 connected pieces of their pieces we need to check if sum of window of 4 is equal to player_num * 4
    // This approach will optimize our tests
    
    int result = 0;

    // Horizontal check for given row
    result = accumulate(board[row].begin(), board[row].begin() + 4, result);

    for (int j = 1; j < board[row].size() - 3; j++)
    {
        result -= board[row][j - 1];
        result += board[row][j + 3];

        if (result == player_num * 4)
            return true;
    }
    
    // vertical check for given column
    // Taking the given column of the board to the array
    // TODO: refactor repeating code lines 85-94 and 101-110
    vector<int> transposed_column = transpose_column(board, col);

    result = accumulate(transposed_column.begin(), transposed_column.begin() + 4, result);
    
    for (int j = 1; j < transposed_column.size() - 3; j++)
    {
        result -= transposed_column[j - 1];
        result += transposed_column[j + 3];

        if (result == player_num * 4)
            return true;
    }
    return false;

    // Diagonal check
    // Idea is to take a diagonal as an array and do the same check as for horizontal check
}

vector<int> transpose_column(vector<vector<int>>& board, int col)
{
    vector<int> transposed_column;

    for (int i = 0; i < board.size(); i++)
    {
        transposed_column[i] = board[i][col];
    }

    return transposed_column;
}
