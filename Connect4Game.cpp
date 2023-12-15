#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int rows = 1;
int cols = 1;
vector<vector<int>> board;

void draw_board(); // draw board on screen
bool put_piece(int colNum, int player_num); // drop a piece into column
bool check_board_in_pos(int row, int col); // check for winning condition for given position in board
bool check_given_boundary(int col, vector<int>& line);
vector<int> transpose_column(int col); // take a column of a board as an array
bool check_topdown_diagonal(int row, int col); // take a diagonal of a board as an array
bool check_downtop_diagonal(int row, int col); // take a diagonal of a board as an array

int main(int argc, char* argv[])
{    
    constexpr int first_player_num = 1;
    constexpr int second_player_num = 2;
    int current_player_num = 2;
    bool is_game_on = true;
    
    cout << "Enter size of game board (format: rows cols) -> ";
    cin >> cols >> rows;

    board = vector<vector<int>>(rows, vector<int>(cols));

    while (is_game_on)
    {
        current_player_num = current_player_num == first_player_num ? second_player_num : first_player_num;
        draw_board();

        cout << "Player number " << current_player_num << ". It is your turn!" << endl;
        cout << "Enter the column number where to put a piece -> ";
        int colNum;
        cin >> colNum;

        is_game_on = !put_piece(colNum, current_player_num);
    }

    draw_board();
    
    cout << "Congratulations, Player number " << current_player_num << "!" << endl;
    cout << "Y O U  W I N";
    
    return 0;
}

void draw_board()
{
    cout << "  ";
    for (int i = 0; i < cols; i++)
    {
        cout << i + 1 << " ";
    }
    cout << endl;
    
    // for (int i = rows - 1; i >= 0; i--)
    for (int i = 0; i < rows; i++)
    {
        cout << "| ";
        for (int j = 0; j < cols; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << "|" << endl;
    }
}

bool put_piece(int colNum, int player_num)
{
    colNum--;
    int rowNum = 0;

    // find free place in chosen column
    while (rowNum < board.size() && board[rowNum][colNum] != 0)
        rowNum++;

    board[rowNum][colNum] = player_num;
    
    return check_board_in_pos(rowNum, colNum);
}

bool check_board_in_pos(int row, int col)
{
    // idea is to use window of 4 to check sum of elements around given position at board[row][col]
    // Every player has own unique numbers.
    // To get 4 connected pieces of their pieces we need to check if sum of window of 4 is equal to player_num * 4
    // This approach will optimize our tests

    // Horizontal check for given row
    vector<int> horizontal_line = board[row];
    bool answer = check_given_boundary(col, horizontal_line);
    if (answer) return answer;

    // vertical check for given column
    // Taking the given column of the board to the array
    vector<int> transposed_column = transpose_column(col);

    answer = check_given_boundary(col, transposed_column);
    if (answer) return answer;
    
    // Diagonal check
    // Idea is to take a diagonal as an array and do the same check as for horizontal check

    // Down to top diagonal check
    answer = check_topdown_diagonal(row, col);
    if (answer) return answer;

    // Top to down diagonal check
    answer = check_downtop_diagonal(row, col);
    if (answer) return answer;
    
    return answer;
}

bool check_given_boundary(int col, vector<int>& line)
{
    int counter = 1;
    
    for (int j = 0; j < line.size(); j++)
    {
        if (line[j] == 0) continue;

        if (line[j] == line[j + 1])
            counter++;
        else
            counter = 1;

        if (counter == 4)
            return  true;
    }
    return false;
}

vector<int> transpose_column(int col)
{
    vector<int> transposed_column;

    transposed_column.reserve(board.size());
    for (auto& i : board)
    {
        transposed_column.push_back(i[col]);
    }

    return transposed_column;
}

bool check_topdown_diagonal(int row, int col)
{
    const int row_min = max(row - 3, 0);
    const int col_min = max(col - 3, 0);
    const int row_max = min(row + 3, rows - 1);
    const int col_max = min(col + 3, cols - 1);
    
    const int step_back = min(row, col) - min(row_min, col_min);
    const int step_forward = max(row_max, col_max) - max(row, col);
    const int steps = step_back + step_forward + 1;
    
    if (step_back + step_forward < 4) return false;

    auto transposed_diagonal = vector<int>(steps);

    for(int i = -step_back, j = 0; i < step_forward && j < steps; i++, j++)
    {
        transposed_diagonal[j] = board[row + i][col + i];
    }

    return check_given_boundary(col, transposed_diagonal);
}

bool check_downtop_diagonal(int row, int col)
{
    const int row_min = max(row - 3, 0);
    const int col_min = max(col - 3, 0);
    const int row_max = min(row + 3, rows - 1);
    const int col_max = min(col + 3, cols - 1);
    
    const int step_back = min(row, col) - max(row_min, col_min);
    const int step_forward = min(row_max, col_max) - max(row, col);
    const int steps = step_back + step_forward + 1;
    
    if (step_back + step_forward < 4) return false;

    auto transposed_diagonal = vector<int>(steps);

    for(int i = -step_back, j = 0; i < step_forward && j < steps; i++, j++)
    {
        transposed_diagonal[j] = board[row + i][col + i];
    }

    return check_given_boundary(col, transposed_diagonal);
}
