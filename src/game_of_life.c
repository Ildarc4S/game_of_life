#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define NMAX 25
#define MMAX 80

void gameOfLife();
void gameCycle(int arr[NMAX][MMAX]);
int inputFile(int arr[NMAX][MMAX]);
void render(int rows, int cols, int matrix[rows][cols]);
void copyArray(int arr[NMAX][MMAX], int arrBuff[NMAX][MMAX]);
void changeMatrix(int arr[NMAX][MMAX], int arrBuff[NMAX][MMAX]);
void checkRules(int arr[NMAX][MMAX], int arrBuff[NMAX][MMAX], int i, int j, int sum);
void checkDeath(int arr[NMAX][MMAX], int arrBuff[NMAX][MMAX], int *game_run);
int countNeighbors(int arr[NMAX][MMAX], int i, int j);
void changeTime(long int *time, int *game_run);

int main() {
    gameOfLife();
    return 0;
}

void gameOfLife() {
    int arr[NMAX][MMAX];
    if (inputFile(arr)) {
        gameCycle(arr);
    } else {
        printf("Incorrect input file\n");
    }
}

void gameCycle(int arr[NMAX][MMAX]) {
    initscr();
    noecho();
    int arrBuff[NMAX][MMAX];
    long int time = 100000;
    int game_run = 1;

    copyArray(arrBuff, arr);

    while (game_run) {
        clear();
        render(NMAX, MMAX, arrBuff);
        refresh();
        timeout(2);
        usleep(time);

        changeMatrix(arr, arrBuff);

        checkDeath(arr, arrBuff, &game_run);
        changeTime(&time, &game_run);
        copyArray(arr, arrBuff);
    }
    sleep(4);
    endwin();
    printf("GAME OVER\n");
}

int inputFile(int arr[NMAX][MMAX]) {
    int input_result = 1;
    for (int i = 0; i < NMAX && input_result; i++) {
        for (int j = 0; j < MMAX && input_result; j++) {
            if (scanf("%d", &arr[i][j]) == 1) {
                char ch = getchar();
                if (ch != ' ' && ch != '\n') input_result = 0;
            } else {
                input_result = 0;
            }
        }
    }
    if (!freopen("/dev/tty", "r", stdin)) {
        input_result = 0;
    }
    return input_result;
}

void render(int rows, int cols, int matrix[rows][cols]) {
    printw(" ");
    for (int d = 0; d < cols; d++) {
        printw("-");
    }
    printw("\n");
    for (int i = 0; i < rows; i++) {
        printw("|");
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 1) {
                printw("o");
            } else
                (printw(" "));
        }
        printw("|");
        printw("\n");
    }
    printw(" ");
    for (int d = 0; d < cols; d++) {
        printw("-");
    }
    printw(" \n");
    printw("If you want to increace the speed of the game: press the \"a\"\n");
    printw("If you want to reduce the speed of the game: press the \"z\"\n");
    printw("If you want to exit the games: press the \"q\"\n");
}

void copyArray(int arr[NMAX][MMAX], int arrBuff[NMAX][MMAX]) {
    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < MMAX; j++) {
            arr[i][j] = arrBuff[i][j];
        }
    }
}

void changeMatrix(int arr[NMAX][MMAX], int arrBuff[NMAX][MMAX]) {
    for (int i = 0; i < NMAX; i++) {
        for (int j = 0; j < MMAX; j++) {
            int sum = countNeighbors(arr, i, j);
            checkRules(arr, arrBuff, i, j, sum);
        }
    }
}

void checkRules(int arr[NMAX][MMAX], int arrBuff[NMAX][MMAX], int i, int j, int sum) {
    if (arr[i][j] == 0) {
        if (sum == 3) {
            arrBuff[i][j] = 1;
        }
    } else if (arr[i][j] == 1) {
        if (sum < 2 || sum > 3) {
            arrBuff[i][j] = 0;
        }
    }
}

void checkDeath(int arr[NMAX][MMAX], int arrBuff[NMAX][MMAX], int *game_run) {
    int func_result = 1;
    for (int i = 0; i < NMAX && func_result; i++) {
        for (int j = 0; j < MMAX && func_result; j++) {
            if (arr[i][j] != arrBuff[i][j]) {
                func_result = 0;
            }
        }
    }
    if (func_result == 1) *game_run = 0;
}

int countNeighbors(int arr[NMAX][MMAX], int i, int j) {
    int j_left = (j - 1 + MMAX) % MMAX;
    int j_right = (j + 1) % MMAX;
    int i_down = (i + 1) % NMAX;
    int i_up = (i - 1 + NMAX) % NMAX;

    return arr[i_up][j_left] + arr[i_up][j] + arr[i_up][j_right] + arr[i_down][j_left] + arr[i_down][j] +
           arr[i_down][j_right] + arr[i][j_left] + arr[i][j_right];
}

void changeTime(long int *time, int *game_run) {
    char ch = getch();
    const int MAXTIME = 600000;
    const int MINTIME = 50000;
    const int STEP = 50000;
    switch (ch) {
        case 'z':
            if (*time < MAXTIME) *time += STEP;
            break;
        case 'a':
            if (*time - STEP >= MINTIME) *time -= STEP;
            break;
        case 'q':
            *game_run = 0;
            break;
    }
}
