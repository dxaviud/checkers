#include <iostream>
#include <Windows.h>
#include <string>

bool quit = false;
bool gameOver = false;

enum PieceType {none, man, king};
enum PieceColor {no, black, white};

PieceColor playerTurn = white;

class Piece
{
public:
    int x, y;
    PieceType type;
    PieceColor color;
    Piece(int x, int y, PieceType type, PieceColor color) : x(x), y(y), type(type), color(color) {}
    Piece() {}
    void die()
    {
        type = none;
        color = no;
    }
    void setXY(int newX, int newY)
    {
        x = newX;
        y = newY;
    }
    void promote()
    {
        type = king;
    }
};

Piece board[8][8];

char get_piece_char(PieceType t, PieceColor c)
{
    if (c == black)
    {
        if (t == man)
            return 'G';
        else if (t == king)
            return '@';
    }
    else if (c == white)
    {
        if (t == man)
            return 'O';
        else if (t == king)
            return '0';
    }
    else
    {
        return ' ';
    }
}

std::string get_player_turn()
{
    if (playerTurn == white)
        return "white (O or 0)";
    else
        return "black (G or @)";
}

void display_board()
{
    system("cls");

    std::cout << "Player turn: " << get_player_turn() << "\n" << std::endl;
    
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            if (x == 8)
                std::cout << y;
            else
                std::cout << get_piece_char(board[y][x].type, board[y][x].color) << ' ';
        }
        std::cout << std::endl;
    }
    // std::cout << std::endl;
    for (int x = 0; x < 8; x++)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

void initialize_board()
{
    //initialize the board
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (y % 2 == 0)
            {
                if (x % 2 == 0)
                {
                    if (y < 3)
                        board[y][x] = Piece(x, y, man, black);
                    else if (y > 4)
                        board[y][x] = Piece(x, y, man, white);
                    else
                        board[y][x] = Piece(x, y, none, no);
                }
            }
            else
            {
                if (x % 2 == 1)
                {
                    if (y < 3)
                        board[y][x] = Piece(x, y, man, black);
                    else if (y > 4)
                        board[y][x] = Piece(x, y, man, white);
                    else
                        board[y][x] = Piece(x, y, none, no);
                }
            }
        }
    }
}

void move(int xFrom, int yFrom, int xTo, int yTo)
{
    board[yTo][xTo] = board[yFrom][xFrom];
    board[yTo][xTo].setXY(xTo, yTo);
    board[yFrom][xFrom].die();

    if (board[yTo][xTo].color == white && yTo == 0)
        board[yTo][xTo].promote();
    else if (board[yTo][xTo].color == black && yTo == 7)
        board[yTo][xTo].promote();

    if (xTo > xFrom + 1 || xTo < xFrom - 1 || yTo > yFrom + 1 || yTo < yFrom - 1)
    {
        board[(yFrom + yTo) / 2][(xFrom + xTo) / 2].die();
    }                            
}

PieceColor other_player()
{
    if (playerTurn == white)
        return black;
    else
        return white;
}

bool has_legal_move(int, int);

bool out_of_bounds(int x, int y) {
    return (x < 0 || x > 7 || y < 0 || y > 7);
}

void input()
{
    int x1, y1, x2, y2;

    bool validFirst = false;

    while (!validFirst)
    {
        std::cout << "pick piece to move using 'x-pos y-pos': ";
        std::cin >> x1 >> y1;
        if (std::cin.fail())
        {
            std::cout << "invalid input, try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        if (out_of_bounds(x1, y1))
        {
            std::cout << "out of bounds, try again" << std::endl;
            continue;
        }

        // std::cout << "enter y1:";
        // std::cin >> y1;
        // if (std::cin.fail())
        // {
        //     std::cout << "invalid input, try again" << std::endl;
        //     std::cin.clear();
        //     std::cin.ignore(1000, '\n');
        //     continue;
        // }
        // if (y1 < 0 || y1 > 7)
        // {
        //     std::cout << "out of bounds, try again" << std::endl;
        //     continue;
        // }
        if (!has_legal_move(x1, y1))
        {
            std::cout << "this piece has no legal moves, try again" << std::endl;
            continue;
        }
        validFirst = board[y1][x1].color == playerTurn;
        if (!validFirst)
            std::cout << "Invalid piece chosen, try again" << std::endl;
    }

    bool validSecond = false;

    while(!validSecond)
    {
        std::cout << "enter where to move using 'x-pos y-pos': ";
        std::cin >> x2 >> y2;
        if (std::cin.fail())
        {
            std::cout << "invalid input, try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        if (out_of_bounds(x2, y2))
        {
            std::cout << "out of bounds, try again" << std::endl;
            continue;
        }
        // std::cout << "enter y2:";
        // std::cin >> y2;
        // if (std::cin.fail())
        // {
        //     std::cout << "invalid input, try again" << std::endl;
        //     std::cin.clear();
        //     std::cin.ignore(1000, '\n');
        //     continue;
        // }
        // if (y2 < 0 || y2 > 7)
        // {
        //     std::cout << "out of bounds, try again" << std::endl;
        //     continue;
        // }
        if (board[y1][x1].type == king)
        {
            validSecond = (board[y2][x2].type == none) && ((x2 + 1 == x1 && y2 + 1 == y1) || (x2 + 1 == x1 && y2 - 1 == y1) || (x2 - 1 == x1 && y2 + 1 == y1) || (x2 - 1 == x1 && y2 - 1 == y1) ||
                (x2 + 2 == x1 && y2 + 2 == y1 && board[y2+1][x2+1].color == other_player()) || (x2 + 2 == x1 && y2 - 2 == y1 && board[y2 - 1][x2 + 1].color == other_player()) || (x2 - 2 == x1 && y2 + 2 == y1 && board[y2 + 1][x2 - 1].color == other_player()) || (x2 - 2 == x1 && y2 - 2 == y1 && board[y2 - 1][x2 - 1].color == other_player()));
        }
        else if (board[y1][x1].type == man)
        {
            if (board[y1][x1].color == black)
                validSecond = (board[y2][x2].type == none) && ((x2 + 1 == x1 && y2 - 1 == y1) || (x2 - 1 == x1 && y2 - 1 == y1) ||
                    (x2 + 2 == x1 && y2 - 2 == y1 && board[y2 - 1][x2 + 1].color == other_player()) || (x2 - 2 == x1 && y2 - 2 == y1 && board[y2 - 1][x2 - 1].color == other_player()));
            else if (board[y1][x1].color == white)
                validSecond = (board[y2][x2].type == none) && ((x2 + 1 == x1 && y2 + 1 == y1) || (x2 - 1 == x1 && y2 + 1 == y1) ||
                    (x2 + 2 == x1 && y2 + 2 == y1 && board[y2 + 1][x2 + 1].color == other_player()) || (x2 - 2 == x1 && y2 + 2 == y1 && board[y2 + 1][x2 - 1].color == other_player()));
        }
        
        if (!validSecond)
            std::cout << "Invalid move to position, try again" << std::endl;
    }

    move(x1, y1, x2, y2);
}

void switchPlayerTurn()
{
    if (playerTurn == white)
        playerTurn = black;
    else
        playerTurn = white;
}

bool noMorePieces()
{
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (board[y][x].color == playerTurn)
                return false;
        }
    }
    return true;
}

bool has_legal_move(int x, int y)
{
    if (board[y][x].type == none)
        return false;
    else if (board[y][x].type == king)
    {
        for (int yy = y - 2; yy < y + 3; yy++)
        {
            for (int xx = x - 2; xx < x + 3; xx++)
            {
                if (xx == x || yy == y)
                    continue;
                if (xx - x != yy - y && xx - x != -(yy - y))
                    continue;
                if (xx < 0 || xx > 7)
                    continue;
                if (yy < 0 || yy > 7)
                    continue;

                if (board[yy][xx].type == none)
                {
                    if (abs(yy - y) == 2)
                    {
                        if (board[(yy + y) / 2][(xx + x) / 2].color == other_player())
                            return true;
                        else
                            continue;
                    }
                    else
                        return true;
                }
            }
        }
        return false;
    }
    else
    {
        if (board[y][x].color == black)
        {
            for (int yy = y + 1; yy < y + 3; yy++)
            {
                for (int xx = x - 2; xx < x + 3; xx++)
                {
                    if (xx == x || yy == y)
                        continue;
                    if (xx - x != yy - y && xx - x != -(yy - y))
                        continue;
                    if (xx < 0 || xx > 7)
                        continue;
                    if (yy < 0 || yy > 7)
                        continue;

                    if (board[yy][xx].type == none)
                    {
                        if (abs(yy - y) == 2)
                        {
                            if (board[(yy + y) / 2][(xx + x) / 2].color == other_player())
                                return true;
                            else
                                continue;
                        }
                        else
                            return true;
                    }
                }
            }
            return false;
        }
        else if (board[y][x].color == white)
        {
            for (int yy = y - 2; yy < y; yy++)
            {
                for (int xx = x - 2; xx < x + 3; xx++)
                {
                    if (xx == x || yy == y)
                        continue;
                    if (xx - x != yy - y && xx - x != -(yy - y))
                        continue;
                    if (xx < 0 || xx > 7)
                        continue;
                    if (yy < 0 || yy > 7)
                        continue;
                     
                    if (board[yy][xx].type == none)
                    {
                        if (abs(yy - y) == 2)
                        {
                            if (board[(yy + y) / 2][(xx + x) / 2].color == other_player())
                                return true;
                            else
                                continue;
                        }
                        else
                            return true;
                    }
                }
            }
            return false;
        }
    }
}

bool noMoreMoves()
{
    //iterate over all pieces of the correct color
    //if that piece has any legal move, return false
    //after iterating over all pieces, return true
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (board[y][x].color == playerTurn && has_legal_move(x, y))
                return false;
        }
    }
    return true;

}

void logic()
{
    //check for game over
    if (noMorePieces())
        gameOver = true;
    else if (noMoreMoves())
        gameOver = true;
}


int main()
{
    initialize_board();

    while (!quit)
    {
        display_board();
        logic();
        if (gameOver)
            break;
        input();
        switchPlayerTurn();
    }

    switchPlayerTurn();

    if (gameOver)
    {
        std::cout << "game over! " << get_player_turn() << " wins!";
    }

}

