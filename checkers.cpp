
#include <iostream>
#include <Windows.h>

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


char getPieceChar(PieceType t, PieceColor c)
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

const char* getPlayerTurnString()
{
    if (playerTurn == white)
        return "white";
    else
        return "black";
}

void display()
{
    system("cls");

    std::cout << "Player turn: " << getPlayerTurnString() << "\n" << std::endl;
    
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            if (x == 8)
                std::cout << y;
            else
                std::cout << getPieceChar(board[y][x].type, board[y][x].color) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for (int x = 0; x < 8; x++)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

void init()
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

void move(int x1, int y1, int x2, int y2)
{
#if 0
    if (board[y1][x1].type == none)
    {
        std::cout << "invalid piece selected for movement." << std::endl;
        std::cin.get();
        return;
    }

    
    if (   !(    (x2 + 1 == x1 && y2 + 1 == y1) || (x2 + 1 == x1 && y2 - 1 == y1) || (x2 - 1 == x1 && y2 + 1 == y1) || (x2 - 1 == x1 && y2 - 1 == y1) ||
        (x2 + 2 == x1 && y2 + 2 == y1) || (x2 + 2 == x1 && y2 - 2 == y1) || (x2 - 2 == x1 && y2 + 2 == y1) || (x2 - 2 == x1 && y2 - 2 == y1)   )  )
    {
        std::cout << "can't move to that location." << std::endl;
        std::cin.get();
        return;
    }
    if (!(board[y2][x2].type == none))
    {
        std::cout << "position to move to is occupied." << std::endl;
        std::cin.get();
        return;
    }

#endif
    board[y2][x2] = board[y1][x1];
    board[y2][x2].setXY(x2, y2);
    board[y1][x1].die();

    if (board[y2][x2].color == white && y2 == 0)
        board[y2][x2].promote();
    else if (board[y2][x2].color == black && y2 == 7)
        board[y2][x2].promote();

    if (x2 > x1 + 1 || x2 < x1 - 1 || y2 > y1 + 1 || y2 < y1 - 1)
    {
        board[(y1 + y2) / 2][(x1 + x2) / 2].die();
    }                            
    
        

}

PieceColor otherColor()
{
    if (playerTurn == white)
        return black;
    else
        return white;
}

bool hasLegalMove(int, int);

void input()
{
    int x1, y1, x2, y2;

    bool validFirst = false;

    while (!validFirst)
    {
        std::cout << "enter x1:";
        std::cin >> x1;
        if (std::cin.fail())
        {
            std::cout << "invalid input, try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        if (x1 < 0 || x1 > 7)
        {
            std::cout << "out of bounds, try again" << std::endl;
            continue;
        }

        std::cout << "enter y1:";
        std::cin >> y1;
        if (std::cin.fail())
        {
            std::cout << "invalid input, try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        if (y1 < 0 || y1 > 7)
        {
            std::cout << "out of bounds, try again" << std::endl;
            continue;
        }
        if (!hasLegalMove(x1, y1))
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
        std::cout << "enter x2:";
        std::cin >> x2;
        if (std::cin.fail())
        {
            std::cout << "invalid input, try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        if (x2 < 0 || x2 > 7)
        {
            std::cout << "out of bounds, try again" << std::endl;
            continue;
        }
        std::cout << "enter y2:";
        std::cin >> y2;
        if (std::cin.fail())
        {
            std::cout << "invalid input, try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }
        if (y2 < 0 || y2 > 7)
        {
            std::cout << "out of bounds, try again" << std::endl;
            continue;
        }
        if (board[y1][x1].type == king)
        {
            validSecond = (board[y2][x2].type == none) && ((x2 + 1 == x1 && y2 + 1 == y1) || (x2 + 1 == x1 && y2 - 1 == y1) || (x2 - 1 == x1 && y2 + 1 == y1) || (x2 - 1 == x1 && y2 - 1 == y1) ||
                (x2 + 2 == x1 && y2 + 2 == y1 && board[y2+1][x2+1].color == otherColor()) || (x2 + 2 == x1 && y2 - 2 == y1 && board[y2 - 1][x2 + 1].color == otherColor()) || (x2 - 2 == x1 && y2 + 2 == y1 && board[y2 + 1][x2 - 1].color == otherColor()) || (x2 - 2 == x1 && y2 - 2 == y1 && board[y2 - 1][x2 - 1].color == otherColor()));
        }
        else if (board[y1][x1].type == man)
        {
            if (board[y1][x1].color == black)
                validSecond = (board[y2][x2].type == none) && ((x2 + 1 == x1 && y2 - 1 == y1) || (x2 - 1 == x1 && y2 - 1 == y1) ||
                    (x2 + 2 == x1 && y2 - 2 == y1 && board[y2 - 1][x2 + 1].color == otherColor()) || (x2 - 2 == x1 && y2 - 2 == y1 && board[y2 - 1][x2 - 1].color == otherColor()));
            else if (board[y1][x1].color == white)
                validSecond = (board[y2][x2].type == none) && ((x2 + 1 == x1 && y2 + 1 == y1) || (x2 - 1 == x1 && y2 + 1 == y1) ||
                    (x2 + 2 == x1 && y2 + 2 == y1 && board[y2 + 1][x2 + 1].color == otherColor()) || (x2 - 2 == x1 && y2 + 2 == y1 && board[y2 + 1][x2 - 1].color == otherColor()));
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

bool hasLegalMove(int x, int y)
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
                        if (board[(yy + y) / 2][(xx + x) / 2].color == otherColor())
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
                            if (board[(yy + y) / 2][(xx + x) / 2].color == otherColor())
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
                            if (board[(yy + y) / 2][(xx + x) / 2].color == otherColor())
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
            if (board[y][x].color == playerTurn && hasLegalMove(x, y))
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
    init();

    while (!quit)
    {
        display();
        logic();
        if (gameOver)
            break;
        input();
        switchPlayerTurn();
    }

    switchPlayerTurn();

    if (gameOver)
    {
        std::cout << "game over! " << getPlayerTurnString() << " wins!";
    }

}

