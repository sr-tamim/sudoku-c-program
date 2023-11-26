/**
 * @file sudoku-win.c
 * @brief Sudoku game implementation in C programming
 *
 * This program allows the user to play the game of Sudoku. It provides a
 * command-line interface where the user can input their moves and see the
 * current state of the Sudoku board. The program creates a random Sudoku board every time
 *
 * @date 26-Nov-2023
 * @version 1.0
 * @authors
 * - SR Tamim <https://sr-tamim.vercel.app>
 * 
 * @section LICENSE
 * This program is licensed under MIT License. See the LICENSE file in the root of this repository for details.
 * 
 * @section Dependencies
 * - stdio.h
 * - stdlib.h
 * - stdbool.h
 * - time.h
 * - math.h
 * 
 * @section NOTES
 * This program is tested on Windows 11 using MinGW compiler
 * 
 * @section Usage
 * - Compile the program using C compiler (gcc, clang, mingw, etc)
 * - Run the executable file
 * - You can also download the executable file from the releases section of this repository
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define N 9             // Size of the board
#define MINI_BOX_SIZE 3 // Size of the mini box 3x3
#define EASY_LVL 13      // Number of empty cells for easy level
#define MEDIUM_LVL 29   // Number of empty cells for medium level
#define HARD_LVL 41     // Number of empty cells for hard level

int board[N][N];    // Sudoku board with empty cells
int solvedBoard[N][N];  // Sudoku board with all cells filled
int totalEmptyCells = MEDIUM_LVL;   // Number of empty cells

// Function declarations
void clearScreen();     // clear the screen
int randomGenerator(int num);   // random number generator
bool checkIfSafe(int i, int j, int num);    // check if it is safe to put the number in specific cell
bool isAbsentInBox(int rowStart, int colStart, int num);    // check if the number is absent in the 3x3 box
bool isAbsentInRow(int i, int num);     // check if the number is absent in the row
bool isAbsentInCol(int j, int num);     // check if the number is absent in the column
void fillValues();      // fill the board with values
void fillDiagonal();    // fill the diagonal 3 number of 3x3 boxes
void fillBox(int row, int col);     // fill a 3x3 box
bool fillRemaining(int i, int j);   // fill the remaining cells recursively
void addEmptyCells();   // remove digits from the board to create empty cells
void printSudoku();     // print the sudoku board
bool isBoardSolved();   // check if the board is solved
void resetBoard();      // reset the board to all 0s

/* =========== Main Function =========== */
int main()
{
    // run the program in a loop until the user wants to exit
    do
    {
        srand((unsigned int)time(NULL)); // seed the random number generator

        clearScreen(); // clear the screen

        printf("Welcome to Sudoku!\n\n"); // print welcome message

        // ask for difficulty level
        printf("Choose difficulty level:\n");
        printf("1. Easy\n");
        printf("2. Medium (default)\n");
        printf("3. Hard\n");
        printf("Enter your choice: ");

        int difficultyChoice;
        scanf(" %d", &difficultyChoice); // get the difficulty choice from the user

        // set the total empty cells based on the difficulty choice using switch case
        switch (difficultyChoice)
        {
        case 1:
            totalEmptyCells = EASY_LVL;
            printf("\nEasy level selected\n\n");
            break;
        case 2:
            totalEmptyCells = MEDIUM_LVL;
            printf("\nMedium level selected\n\n");
            break;
        case 3:
            totalEmptyCells = HARD_LVL;
            printf("\nHard level selected\n\n");
            break;
        default:
            totalEmptyCells = MEDIUM_LVL;
            printf("\nMedium level selected\n\n");
            break;
        }

        resetBoard(); // reset the board
        fillValues(); // fill the board with values
        printSudoku(); // print the board

        // ask for row, column and value from the user
        // and also save the number of attempts
        int row, col, num, attempts = 0;

        while (!isBoardSolved()) // run the loop until the board is solved
        {
            printf("Enter column (X axis): ");
            scanf("%d", &col); // get column number from the user

            printf("Enter row (Y axis): ");
            scanf("%d", &row); // get row number from the user

            // check if the row and column are valid
            if (row > 9 || col > 9 || row < 1 || col < 1)
            {
                // if not valid then ask the user if they want to try again
                printf("Invalid row or column! Try again? (y/n) ");
                char c;
                scanf(" %c", &c); // get the choice from the user
                if (c == 'N' || c == 'n')
                    goto exit; // if the user doesn't want to try again then exit the program
                else
                    continue; // if the user wants to try again then continue the loop
            }

            // decrement the row and column by 1 to get the correct index
            // because the board starts from 0
            row--;
            col--;

            // check if the cell is already filled
            if (board[row][col] != 0)
            {
                // if the cell is already filled then ask the user if they want to try again
                printf("This cell is already filled! Try again? (y/n) ");
                char c;
                scanf(" %c", &c); // get the choice from the user
                if (c == 'N' || c == 'n')
                    goto exit; // if the user doesn't want to try again then exit the program
                else
                    continue; // if the user wants to try again then continue the loop
            }

        // ask the user to enter the value
        enterValue: // label to go to if the user wants to try again
            printf("Enter value: ");
            scanf("%d", &num); // get the value from the user

            // check if the value is valid
            if (num > 9 || num < 1)
            {
                // if not valid then ask the user if they want to try again
                printf("Invalid value! Try again? (y/n) ");
                char c;
                scanf(" %c", &c); // get the choice from the user
                if (c == 'N' || c == 'n')
                    goto exit; // if the user doesn't want to try again then exit the program
                else
                    goto enterValue; // if the user wants to try again then go to enterValue label
            }

            attempts++; // increment the number of attempts

            // check if the value is safe to put in the cell
            if (checkIfSafe(row, col, num))
                board[row][col] = num; // if safe then put the value in the cell
            else
            {
                // if not safe then ask the user if they want to try again
                printf("Invalid value! Try again? (y/n) ");
                char c;
                scanf(" %c", &c); // get the choice from the user
                if (c == 'N' || c == 'n')
                    goto exit; // if the user doesn't want to try again then exit the program
                // else continue the loop
            }
            clearScreen(); // clear the screen

            // print the number of attempts and the board
            printf("Attempted %d times\n\n", attempts);
            printSudoku();
        }
        // while loop ends here when the board is solved

        // print congratulations message
        printf("\nCongratulations! You solved the board!\n\n");

    // ask the user if they want to play again
    askForPlayAgain:
        printf("Do you want to play again? (y/n) ");
        char c;
        scanf(" %c", &c); // get the choice from the user
        if (c == 'Y' || c == 'y')
            continue; // if the user wants to play again then continue the loop
        else if (c != 'N' && c != 'n')
            goto askForPlayAgain; // if the user enters an invalid choice then go to askForPlayAgain label
        else
            break; // if the user doesn't want to play again then exit the program
    } while (true); // run the program in an infinite loop until the user wants to exit

// exit the program
exit:
    return 0; // return 0 to indicate successful execution
}
/* =========== End of Main Function =========== */


/* =========== User Defined Functions =========== */

// clear the screen
void clearScreen()
{
    system("cls");
}


// Random generator
int randomGenerator(int num)
{
    return (int)((double)((float)rand() / RAND_MAX * num) + 1);
}

// Check if safe to put in cell
bool checkIfSafe(int i, int j, int num)
{
    return (isAbsentInRow(i, num) && isAbsentInCol(j, num) && isAbsentInBox(i - i % MINI_BOX_SIZE, j - j % MINI_BOX_SIZE, num));
}

// Returns false if given 3 x 3 block contains num.
bool isAbsentInBox(int rowStart, int colStart, int num)
{
    // rowStart and colStart will give the starting cell of the box
    // check for num in the box
    // i and j will give the cell position
    for (int i = 0; i < MINI_BOX_SIZE; i++)
    {
        for (int j = 0; j < MINI_BOX_SIZE; j++)
        {
            // if the number is found in the box then return false
            if (board[rowStart + i][colStart + j] == num)
            {
                return false;
            }
        }
    }
    return true;    // returns true if num is not found in the box
}

// check in the row for existence
bool isAbsentInRow(int i, int num)
{
    for (int j = 0; j < N; j++)
    {
        // if the number is found in the row then return false
        if (board[i][j] == num)
        {
            return false;
        }
    }
    return true; // returns true if num is not found in the row
}

// check in the row for existence
bool isAbsentInCol(int j, int num)
{
    for (int i = 0; i < N; i++)
    {
        // if the number is found in the column then return false
        if (board[i][j] == num)
        {
            return false;
        }
    }
    return true; // returns true if num is not found in the column
}

// Fill the board with values
void fillValues()
{
    fillDiagonal(); // Fill the diagonal MINI_BOX_SIZE x MINI_BOX_SIZE matrices
    fillRemaining(0, MINI_BOX_SIZE);    // Fill remaining blocks

    // Copy the solved board to board
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            solvedBoard[i][j] = board[i][j];
    }
    addEmptyCells();    // remove the K no. of digits from the board
}

// Fill the diagonal MINI_BOX_SIZE number of MINI_BOX_SIZE x MINI_BOX_SIZE matrices
void fillDiagonal()
{
    // for diagonal box,
    // i + MINI_BOX_SIZE represents the diagonal number
    for (int i = 0; i < N; i = i + MINI_BOX_SIZE)
    {
        fillBox(i, i);  // Fill a 3 x 3 matrix
    }
}

// Fill a 3 x 3 matrix.
void fillBox(int row, int col)
{
    int num;
    for (int i = 0; i < MINI_BOX_SIZE; i++)
    {
        for (int j = 0; j < MINI_BOX_SIZE; j++)
        {
            do
            {
                num = randomGenerator(N);
            } while (!isAbsentInBox(row, col, num));
            board[row + i][col + j] = num;
        }
    }
}

// diagnoal box is filled, below function will fill the rest of the cells recursively
// A recursive function to fill remaining matrix
bool fillRemaining(int i, int j) // i is row and j is column
{
    // if all column is filled then move to next row
    if (j >= N && i < N - 1)
    {
        i = i + 1; // move to next row
        j = 0; // column starts from 0 in new row
    }

    // if all row and columns are filled then return true
    if (i >= N && j >= N)
    {
        // here recursion ends
        return true; // board is filled
    }

    // adjust column based on mini box size
    if (i < MINI_BOX_SIZE) // for first 3 rows of the board
    {
        // first 3 columns of first 3 rows are already filled
        // so move to fourth column if j < 3
        if (j < MINI_BOX_SIZE)
        {
            j = MINI_BOX_SIZE;
        }
    }
    else if (i < N - MINI_BOX_SIZE) // for 4th row to 6th row
    {
        if (j == (int)(i / MINI_BOX_SIZE) * MINI_BOX_SIZE)
        {
            j = j + MINI_BOX_SIZE;
        }
    }
    else // for last 3 rows
    {
        // last 3 columns of last 3 rows are already filled
        // so if j enters 7th column then move to next row
        if (j == N - MINI_BOX_SIZE)
        {
            i = i + 1; // move to next row
            j = 0; // column starts from 0 in new row

            if (i >= N) // if i crosses the board size then return true and end the recursion
            {
                // here recursion ends
                return true; // board is filled
            }
        }
    }

    // fill the board with remaining numbers
    for (int num = 1; num <= N; num++)
    {
        if (checkIfSafe(i, j, num)) // check if it is safe to put the number in the cell
        {
            board[i][j] = num; // put the number in the cell

            if (fillRemaining(i, j + 1)) // fill the remaining cells recursively
            {
                return true; // board is filled
            }
            board[i][j] = 0;
        }
    }
    return false; // board is not filled
}

// Remove some digits from the board to create empty cells
void addEmptyCells()
{
    int count = totalEmptyCells;
    while (count != 0)
    {
        // Generate a random number between 0 and 80
        int cellId = randomGenerator(N * N) - 1;
        int i = (cellId / N); // get row number by dividing by N
        int j = cellId % N; // get column number by moduling with N

        if (j != 0) // if j is not 0 then decrement j by 1 to get the correct column number
        {
            j = j - 1;
        }

        if (board[i][j] != 0) // if the cell is not empty then remove the number from the cell
        {
            count--; // decrement the count
            board[i][j] = 0; // remove the number from the cell
        }
    }
}

// Print sudoku board
void printSudoku()
{
    // print the column numbers on top
    for (int i = 1; i <= N; i++)
    {
        if (i == 1)
            printf("  X");
        printf(" %d", i);
        if (i % 3 == 0)
            printf("  ");
    }

    // print a line under the column numbers
    printf("\nY -------------------------\n");

    // print the board
    for (int i = 0; i < N; i++)
    {
        // print a line after every 3 rows
        if (i != 0 && i % 3 == 0)
            printf("  -------------------------\n");

        for (int j = 0; j < N; j++)
        {
            // print the row numbers on the left
            if (j == 0)
                printf("%d | ", i + 1);
            
            // print the cell value
            printf("%d ", board[i][j]);

            // print a line after every 3 columns
            if ((j + 1) % 3 == 0)
                printf("| ");
        }
        printf("\n"); // print a new line after every row
    }
    // print a line under the board
    printf("  -------------------------\n");
}

// Check if the board is solved
bool isBoardSolved()
{
    // compare the board with the solved board
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            if (board[i][j] != solvedBoard[i][j])
                return false; // return false if any cell is not equal
    }
    return true; // return true if all cells are equal
}

// Reset the board
void resetBoard()
{
    // reset all cells to 0
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            board[i][j] = 0;
    }
}
