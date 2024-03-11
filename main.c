/*		LIBRARY		*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

/*		MACRO		*/
#define UP_ARROW 72
#define LEFT_ARROW 75
#define RIGHT_ARROW 77
#define DOWN_ARROW 80
#define ENTER_KEY 13

#define ROWS 5
#define COLUMNS 6
#define MAX_BUBBLE_COUNT 7

#define EMPTY_SPACE 0
#define EMPTY_SPACE_CHAR " "
#define EMPTY_BOARD_SPACE_CHAR "⚫" // ⬛

#define BUBBLE_STATE_1 1
#define BUBBLE_STATE_2 2
#define BUBBLE_STATE_3 3

#define BUBBLE_STATE_1_CHAR "🔵"
#define BUBBLE_STATE_2_CHAR "🟡"
#define BUBBLE_STATE_3_CHAR "🔴"

#define BOX_CORNER_1 "┌"
#define BOX_CORNER_2 "┐"
#define BOX_CORNER_3 "└"
#define BOX_CORNER_4 "┘"

/*		ENUM		*/
enum Difficulty
{
    EASY = 5,
    MEDIUM = 3,
    HARD = -1
};

/*		FUNCTION DECLARATION	c	*/
void clearScreen(void);
void printInPosXY(int x, int y, char const *str);
void loadGame(int difficulty);
int calculateMovesToWin(int board[ROWS][COLUMNS]);
void burstBubble(int board[ROWS][COLUMNS], int row, int col);
int bubbleCountOnBoard(int board[ROWS][COLUMNS]);
void selectBubble(int currentSelection[2]);
void displayMovesLeft(int movesLeft);
void displayGameBoard(int board[ROWS][COLUMNS]);
void populateGameBoard(int board[ROWS][COLUMNS]);
void initEmptyGameBoard(int board[ROWS][COLUMNS]);
void displayWelcomeScreen(void);
int setDifficulty(void);
int mainMenu(void);
int menuSelector(int x, int y, int yStart);
void matriscopy(void *destmat, void *srcmat);
char waitForAnyKey(void);

/*		FUNCTION DECLARATION		*/
int main(void)
{
    int difficulty = EASY;

    // set console code page to utf-8
    // https://stackoverflow.com/a/48977245
    SetConsoleOutputCP(65001);

    displayWelcomeScreen();

    do
    {
        switch (mainMenu())
        {
        case 0:
            loadGame(difficulty);
            break;
        case 1:
            difficulty = setDifficulty();
            break;
        case 2:
            exit(EXIT_SUCCESS);
            break;
        }
    } while (1);

    return 0;
}

void clearScreen(void)
{
    system("@cls||clear");
}

void printInPosXY(int x, int y, char const *str)
{
    printf("%c[%d;%df", 0x1B, y, x);
    printf(str);
}

char waitForAnyKey(void)
{
    while (!kbhit())
        ;
    return (char)getch();
}

void matriscopy(void *destmat, void *srcmat)
{
    memcpy(destmat, srcmat, ROWS * COLUMNS * sizeof(int));
}

int menuSelector(int x, int y, int yStart)
{
    char key;
    int selected = 0;
    x = x - 2; // così stampo a sinistra del menu di selezione
    printInPosXY(x, yStart + selected, ">");
    do
    {
        key = waitForAnyKey();
        switch (key)
        {
        case (char)UP_ARROW:
            printInPosXY(x, yStart + selected, " ");

            if (yStart >= yStart + selected)
                selected = y - yStart - 1;
            else
                selected--;

            printInPosXY(x, yStart + selected, ">");
            break;

        case (char)DOWN_ARROW:
            printInPosXY(x, yStart + selected, " ");

            if (selected + 1 >= y - yStart)
                selected = 0;
            else
                selected++;
            printInPosXY(x, yStart + selected, ">");

            break;
        default:
            break;
        }

    } while (key != (char)ENTER_KEY);

    return (selected);
}

int mainMenu(void)
{
    int x = 10, y = 3;
    int yStart = y;

    clearScreen();

    printInPosXY(x, y - 1, "Game Menu");
    printInPosXY(x, y++, "New Game\n");
    printInPosXY(x, y++, "Set difficulty\n");
    printInPosXY(x, y++, "Exit\n");

    return menuSelector(x, y, yStart);
}

int setDifficulty(void)
{
    int x = 10, y = 3;
    int yStart = y;

    clearScreen();
    printInPosXY(x, y - 1, "Select difficulty");
    printInPosXY(x, y++, "Easy\n");
    printInPosXY(x, y++, "Medium\n");
    printInPosXY(x, y++, "Hard\n");

    switch (menuSelector(x, y, yStart))
    {
    case 0:
        return EASY;
        break;
    case 1:
        return MEDIUM;
        break;
    case 2:
        return HARD;
        break;

    default:
        EASY;
        break;
    }

    return EASY;
}

void initEmptyGameBoard(int board[ROWS][COLUMNS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            board[i][j] = EMPTY_SPACE;
        }
    }

    return;
}

void displayWelcomeScreen(void)
{
    clearScreen();

    printf("/$$$$$$$            /$$       /$$       /$$                 /$$$$$$$  /$$                       /$$ \n");
    printf("| $$__  $$          | $$      | $$      | $$                | $$__  $$| $$                      | $$    \n");
    printf("| $$  \\ $$ /$$   /$$| $$$$$$$ | $$$$$$$ | $$  /$$$$$$       | $$  \\ $$| $$  /$$$$$$   /$$$$$$$ /$$$$$$  \n");
    printf("| $$$$$$$ | $$  | $$| $$__  $$| $$__  $$| $$ /$$__  $$      | $$$$$$$ | $$ |____  $$ /$$_____/|_  $$_/  \n");
    printf("| $$__  $$| $$  | $$| $$  \\ $$| $$  \\ $$| $$| $$$$$$$$      | $$__  $$| $$  /$$$$$$$|  $$$$$$   | $$    \n");
    printf("| $$  \\ $$| $$  | $$| $$  | $$| $$  | $$| $$| $$_____/      | $$  \\ $$| $$ /$$__  $$ \\____  $$  | $$ /$$\n");
    printf("| $$$$$$$/|  $$$$$$/| $$$$$$$/| $$$$$$$/| $$|  $$$$$$$      | $$$$$$$/| $$|  $$$$$$$ /$$$$$$$/  |  $$$$/\n");
    printf("|_______/  \\______/ |_______/ |_______/ |__/ \\_______/      |_______/ |__/ \\_______/|_______/    \\___/  \n");

    printf("\t\t\nPress any key to start...");
    getchar();
}

void populateGameBoard(int board[ROWS][COLUMNS])
{
    initEmptyGameBoard(board);
    int bubbleCount = 0;
    srand((unsigned)time(NULL));

    while (bubbleCount < MAX_BUBBLE_COUNT)
    {
        int bubbleState = rand() % 3 + 1;
        int posX = rand() % ROWS;
        int posY = rand() % COLUMNS;
        if (board[posX][posY] == EMPTY_SPACE)
        {
            board[posX][posY] = bubbleState;
            bubbleCount++;
        }
    }

    return;
}

void displayGameBoard(int board[ROWS][COLUMNS])
{
    int y = 3;

    for (int i = 0; i < ROWS; i++)
    {
        int x = 3;

        for (int j = 0; j < COLUMNS; j++)
        {
            int x2 = x + 6 * (j + 1);
            switch (board[i][j])
            {
            case 1:
                printInPosXY(x2, y, BUBBLE_STATE_1_CHAR);
                break;
            case 2:
                printInPosXY(x2, y, BUBBLE_STATE_2_CHAR);
                break;
            case 3:
                printInPosXY(x2, y, BUBBLE_STATE_3_CHAR);
                break;
            default:
                printInPosXY(x2, y, EMPTY_BOARD_SPACE_CHAR);
                break;
            }
        }
        y += 2;
    }

    return;
}

void displayMovesLeft(int movesLeft)
{
    printInPosXY(1, 14, "");
    printf("Moves left: %d     ", movesLeft);
}

void selectBubble(int currentSelection[2])
{
    char key;
    int x = 8;
    int y = 3;

    int currentSelectedX = currentSelection[1] + x;
    int currentSelectedY = currentSelection[0] + y;

    int tY = y - 1;
    int bY = y + 1;
    int rX = x + 4;
    int lX = x;

    printInPosXY(lX + (5 * (currentSelectedX - x)), tY + (2 * (currentSelectedY - y)), BOX_CORNER_1);
    printInPosXY(rX + (5 * (currentSelectedX - x)), tY + (2 * (currentSelectedY - y)), BOX_CORNER_2);
    printInPosXY(lX + (5 * (currentSelectedX - x)), bY + (2 * (currentSelectedY - y)), BOX_CORNER_3);
    printInPosXY(rX + (5 * (currentSelectedX - x)), bY + (2 * (currentSelectedY - y)), BOX_CORNER_4);

    do
    {
        key = waitForAnyKey();

        printInPosXY(lX + (5 * (currentSelectedX - x)), tY + (2 * (currentSelectedY - y)), EMPTY_SPACE_CHAR);
        printInPosXY(rX + (5 * (currentSelectedX - x)), tY + (2 * (currentSelectedY - y)), EMPTY_SPACE_CHAR);
        printInPosXY(lX + (5 * (currentSelectedX - x)), bY + (2 * (currentSelectedY - y)), EMPTY_SPACE_CHAR);
        printInPosXY(rX + (5 * (currentSelectedX - x)), bY + (2 * (currentSelectedY - y)), EMPTY_SPACE_CHAR);

        switch (key)
        {
        case (char)UP_ARROW:
            if (currentSelectedY > y)
                currentSelectedY--;
            break;
        case (char)DOWN_ARROW:
            if (currentSelectedY < y + ROWS - 1)
                currentSelectedY++;
            break;
        case (char)RIGHT_ARROW:
            if (currentSelectedX < x + COLUMNS - 1)
                currentSelectedX++;
            break;
        case (char)LEFT_ARROW:
            if (currentSelectedX > x)
                currentSelectedX--;
            break;
        default:
            break;
        }

        printInPosXY(lX + (5 * (currentSelectedX - x)), tY + (2 * (currentSelectedY - y)), BOX_CORNER_1);
        printInPosXY(rX + (5 * (currentSelectedX - x)), tY + (2 * (currentSelectedY - y)), BOX_CORNER_2);
        printInPosXY(lX + (5 * (currentSelectedX - x)), bY + (2 * (currentSelectedY - y)), BOX_CORNER_3);
        printInPosXY(rX + (5 * (currentSelectedX - x)), bY + (2 * (currentSelectedY - y)), BOX_CORNER_4);

    } while (key != (char)ENTER_KEY);

    // currentSelectedY becomes current selected x to access correct matrix value and viceversa!
    currentSelection[0] = currentSelectedY - y;
    currentSelection[1] = currentSelectedX - x;
}

int bubbleCountOnBoard(int board[ROWS][COLUMNS])
{
    int bubbleCount = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            if (board[i][j] != EMPTY_SPACE)
                bubbleCount++;
        }
    }

    return bubbleCount;
}

void burstBubble(int board[ROWS][COLUMNS], int row, int col)
{
    if (board[row][col] == EMPTY_SPACE)
        return;
    if (board[row][col] != BUBBLE_STATE_3)
        board[row][col]++;
    else
    {
        board[row][col] = EMPTY_SPACE;
        if (row > 0)
            burstBubble(board, row - 1, col);
        if (row < ROWS - 1)
            burstBubble(board, row + 1, col);
        if (col > 0)
            burstBubble(board, row, col - 1);
        if (col < COLUMNS - 1)
            burstBubble(board, row, col + 1);
    }
}

int calculateMovesToWin(int board[ROWS][COLUMNS])
{
    int tempBoard[ROWS][COLUMNS];
    matriscopy(tempBoard, board);
    int moveCount = 0;

    displayGameBoard(tempBoard);

    do
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLUMNS; j++)
            {
                if (tempBoard[i][j] != EMPTY_SPACE)
                {
                    burstBubble(tempBoard, i, j);
                    moveCount++;
                    ;
                }
            }
        }

    } while (bubbleCountOnBoard(tempBoard) > 0);

    return moveCount;
}

void loadGame(int difficulty)
{
    int gameOver = 0;
    int highScore = 0;
    int currentSelection[] = {0, 0};
    int board[ROWS][COLUMNS];
    int movesLeft = 0;

    clearScreen();
    populateGameBoard(board);
    movesLeft = calculateMovesToWin(board) + difficulty;
    displayGameBoard(board);
    displayMovesLeft(movesLeft);

    do
    {
        selectBubble(currentSelection);
        int currentSelectedX = currentSelection[0];
        int currentSelectedY = currentSelection[1];

        if (board[currentSelectedX][currentSelectedY] >= BUBBLE_STATE_1 && board[currentSelectedX][currentSelectedY] <= BUBBLE_STATE_3)
        {
            burstBubble(board, currentSelectedX, currentSelectedY);
            movesLeft--;
        }

        displayGameBoard(board);
        displayMovesLeft(movesLeft);
        int bubbleCount = bubbleCountOnBoard(board);

        if (bubbleCount == 0 && movesLeft >= 0)
            gameOver = 1;
        if (bubbleCount == 0 && movesLeft < 0)
            gameOver = 99;

    } while (!gameOver);

    if (gameOver == 99)
    {
        printInPosXY(1, 14, "");
        printf("You burst all the bubbles, but with %d more moves... \n", movesLeft * -1);
        printf("Try again, wish you luck 😃");
    }

    if (gameOver == 1)
    {
        printInPosXY(1, 14, "");
        printf("You won 🎉   ! ! !");
    }

    printf("\nPress any key to return back to menu...");
    getchar();
    return;
}
