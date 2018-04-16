/*
    C++ Snake Game for Windows Console
    Made by: Ricardo Arcos
    Last changed: 02/07/2018
*/
#include <iostream>
#include <cstdlib>  // For rand
#include <ctime>    // and srand(time())
#include <conio.h>  // For the sleep() function
#include <windows.h>// For the clear screen function
using namespace std;

// Set to true to end the game.
bool gameOver = false;
// Track the score of player.
// Initialize speed no more than 300.
int score = 0, speed = 200;
// Size of the game map.
const int row = 10;
const int column = 20;
// Game map is represented as
// a two-dimensional char array.
char snakeMap[row][column];
const int tailSize = (row-2)*(column-2)*2;
int tail[tailSize];
// Global variables used in several logical functions for the game.
int headRow, headColumn, fruitRow, fruitColumn, tailCounter = 0;
// Enumerator is used to track the user input.
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton dir;

// Make sure the fruit lands inside the map
// and also somewhere it is allowed to spawn in.
void getFruit()
{
    do{
        fruitRow = rand()%(row-2)+1;
        fruitColumn = rand()%(column-2)+1;
    }while(snakeMap[fruitRow][fruitColumn] != ' ');
}

// Check if we have crashed with anything
// If we crashed with the fruit, get a new fruit.
void checkCrash()
{
    if(snakeMap[headRow][headColumn] == 'F')
    {
        if(speed >= 30)
            speed -= 10;
        else
            speed = 20;

        score += 100;
        tailCounter++;
        getFruit();
    }
    else if(snakeMap[headRow][headColumn] != ' ')
        gameOver = true;

    snakeMap[fruitRow][fruitColumn] = 'F';
}

void updateTail()
{
    // Push the tail coordinates one index to the right
    for(int i = tailCounter*2; i >= 0; i=i-2)
    {
        tail[i+2] = tail[i];
        tail[i+3] = tail[i+1];
    }
    // Once we have pushed all the data one index,
    // update the first two indexes.
    tail[0] = headRow;
    tail[1] = headColumn;
    // Erase the "tip" of the tail
    snakeMap[ tail[tailCounter*2] ][ tail[tailCounter*2+1] ] = ' ';

}// end of updateTail()

// Run this function first before game-play loop
void setupMap()
{
    // start game moving right
    // reset globals to default
    gameOver = false;
    dir = RIGHT;
    score = 0;
    speed = 200;
    tailCounter = 0;
    srand(time(NULL));

    // reset tail
    for(int i = 0; i < tailSize; i++)
        tail[i] = 0;

    // Draw edges in array first
    for(int i = 0; i < row; i++)
        for(int j = 0; j < column; j++)
        {
            if(i == 0 || i == row-1 || j == 0 || j == column-1)
            {
                snakeMap[i][j] = 'X';
            }
            else
                snakeMap[i][j] = ' ';
        } // end inner J for

    // set up snake position
    // draw snake head on middle of array
    // use two integers to keep track of the position
    headRow = row/2;
    headColumn = column/2;
    snakeMap[headRow][headColumn] = 'O';
    // keep track of the tip of the tail using variables
    // similar to the head of the snake
    // later in logic(), we erase the 'O' drawn
    // at the location of the tip
    getFruit();

} // end setupMap()

// I chose to use an 2D array because console performs better when
// the logic is handled in the back end instead of at the same time
// the map is being drawn.
void draw()
{
    system("cls");
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
            cout << snakeMap[i][j];
        cout << endl;
    }
    cout << endl << "SCORE: " << score << endl;
} // end draw()

// Make sure the snake can't do a instant 180 degree turn.
// Allow the user to end the game if he/she presses 'x'.
void input()
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'a':
            if(dir != RIGHT)dir = LEFT;
            break;
        case 'd':
            if(dir != LEFT)dir = RIGHT;
            break;
        case 'w':
            if(dir != DOWN)dir = UP;
            break;
        case 's':
            if(dir != UP)dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
} // end input

void logic()
{

    switch (dir)
    {
    case LEFT:
        updateTail();
        headColumn--;
        checkCrash();
        snakeMap[headRow][headColumn] = 'O';
        break;
    case RIGHT:
        updateTail();
        headColumn++;
        checkCrash();
        snakeMap[headRow][headColumn] = 'O';
        break;
    case UP:
        updateTail();
        headRow--;
        checkCrash();
        snakeMap[headRow][headColumn] = 'O';
        break;
    case DOWN:
        updateTail();
        headRow++;
        checkCrash();
        snakeMap[headRow][headColumn] = 'O';
        break;
    default:
        break;
    }


}

int main()
{
    bool again = true;

    do{
        setupMap();
        while(!gameOver)
        {
            input();
            draw();
            logic();
            if(dir == UP || dir == DOWN)
                Sleep(speed + speed/2);
            else
                Sleep(speed);
        }
        cout << endl << "GAME OVER!" << endl;
        Sleep(1000);

        cout << "Play again? (y/n): ";
        char i;
        cin >> i;
        if(i == 'n' || i == 'N')
            break;
        setupMap();
    }while(again);
    return 0;
}
