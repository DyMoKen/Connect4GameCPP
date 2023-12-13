#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int rows = 1;
int cols = 1;
vector<vector<int>> board;

void draw_board(vector<vector<int>>& board); // draw board on screen
bool put_piece(int colNum, vector<vector<int>>& board, int player_num); // drop a piece into column
bool check_board_in_pos(int row, int col, vector<vector<int>>& board, int player_num); // check for winning condition for given position in board
bool check_given_boundary(int col, vector<int>& line, int player_num);
vector<int> transpose_column(vector<vector<int>>& board, int col); // take a column of a board as an array

int main(int argc, char* argv[])
{
    constexpr int first_player_num = 1;
    constexpr int second_player_num = 2;
    int current_player_num = 2;
    bool is_game_on = true;
    
    cout << "Enter size of game board (format: rows cols) -> ";
    cin >> cols >> rows;

    for (int i = 0; i < rows; i++)
    {
        board.emplace_back();
        for (int j = 0; j < cols; j++)
        {
            board[i].push_back(0);
        }
    }

    draw_board(board);

    while (is_game_on)
    {
        system("cls");
        current_player_num = current_player_num == first_player_num ? second_player_num : first_player_num;
        draw_board(board);

        cout << "Player number " << current_player_num << ". It is your turn!" << endl;
        cout << "Enter the column number where to put a piece -> ";
        int colNum;
        cin >> colNum;

        is_game_on = !put_piece(colNum, board, current_player_num);
    }

    cout << "Congratulations, Player number " << current_player_num << "!" << endl;
    cout << "Y O U  W I N";
    
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

bool put_piece(int colNum, vector<vector<int>>& board, int player_num)
{
    colNum--;
    int rowNum = 0;

    // find free place in chosen column
    while (rowNum < board.size() && board[rowNum][colNum] != 0)
        rowNum++;

    board[rowNum][colNum] = player_num;

    draw_board(board);
    
    return check_board_in_pos(rowNum, colNum, board, player_num);
}

bool check_board_in_pos(int row, int col, vector<vector<int>>& board, int player_num)
{
    // idea is to use window of 4 to check sum of elements around given position at board[row][col]
    // Every player has own unique numbers.
    // To get 4 connected pieces of their pieces we need to check if sum of window of 4 is equal to player_num * 4
    // This approach will optimize our tests

    // Horizontal check for given row
    vector<int> horizontal_line = board[row];
    bool answer = check_given_boundary(col, horizontal_line, player_num);
    if (answer)
        return answer;


    // vertical check for given column
    // Taking the given column of the board to the array
    vector<int> transposed_column = transpose_column(board, col);

    answer = check_given_boundary(col, transposed_column, player_num);
    if (answer)
        return answer;
    // Diagonal check
    // Idea is to take a diagonal as an array and do the same check as for horizontal check
    
    return answer;
}

bool check_given_boundary(int col, vector<int>& line, int player_num)
{
    const int min_boundary = max(col - 3, 0);
    const int max_boundary = min(col + 3, cols - 2);
    int result = 0;
    result = accumulate(line.begin() + min_boundary, line.begin() + min_boundary + 4, result);

    for (int j = min_boundary + 1; j < max_boundary - 3; j++)
    {
        if (result == player_num * 4)
            return true;
        
        result -= line[j - 1];
        result += line[j + 3];

    }
    return false;
}

vector<int> transpose_column(vector<vector<int>>& board, int col)
{
    vector<int> transposed_column;

    for (auto& i : board)
    {
        transposed_column.push_back(i[col]);
    }

    return transposed_column;
}
