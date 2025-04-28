/*
 * 此demo不提供读取自定义文件操作，不提供选择界面，仅作简单示例。 
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>   /* <-- add for system() */

#ifdef _WIN32
#include <conio.h>      /* _getch() */
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
static int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

#define W 8
#define H 7

static char map[H][W + 1] = {
    "######",
    "# .  ###",
    "#    $ #",
    "###  @ #",
    "  ######"
};

/* Find player coords */
static void get_player(int *px, int *py) {
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            if (map[y][x] == '@' || map[y][x] == '+') {
                *px = x; *py = y; return;
            }
}

/* Render map & basic UI */
static void render(int moves) {
//#ifdef _WIN32
//    system("cls");
//#else
//    system("clear");
//#endif
    for (int y = 0; y < H; ++y)
        printf("%s\n", map[y]);
    printf("\nMoves: %d   (WASD to move, Q to quit)\n", moves);
}

/* Check win: no remaining boxes '$' */
static bool is_win(void) {
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            if (map[y][x] == '$') return false;
    return true;
}

static void move_player(int dx, int dy) {
    int px, py;
    get_player(&px, &py);

    int tx = px + dx, ty = py + dy;           /* target */
    int bx = tx + dx, by = ty + dy;           /* beyond (for box) */

    char at_target = map[ty][tx];
    if (at_target == '#') return;             /* wall */

    /* handle box */
    if (at_target == '$' || at_target == '*') {
        char beyond = map[by][bx];
        if (beyond != ' ' && beyond != '.') return; /* blocked */

        /* move box */
        map[by][bx] = (beyond == '.') ? '*' : '$';
        map[ty][tx] = (at_target == '*') ? '.' : ' ';
    }

    /* move player */
    bool was_on_goal = (map[py][px] == '+');
    map[py][px] = was_on_goal ? '.' : ' ';
    map[ty][tx] = (map[ty][tx] == '.') ? '+' : '@';
}

int main(void) {
	printf("注：此demo不提供读取自定义文件操作，不提供选择界面，仅作简单示例。\n");
    int moves = 0;
    char mo[101]; 
    int i = 0;
    render(moves);

    while (1) {
#ifdef _WIN32
        int ch = _getch();
#else
        int ch = getch();
#endif
        if (ch == 'q' || ch == 'Q') break;

        int dx = 0, dy = 0;
        switch (ch) {
            case 'w': case 'W': dy = -1; mo[i++] = 'W'; break;
            case 's': case 'S': dy =  1; mo[i++] = 'S'; break;
            case 'a': case 'A': dx = -1; mo[i++] = 'A'; break;
            case 'd': case 'D': dx =  1; mo[i++] = 'D'; break;
            default: continue; /* ignore others */
        }
        move_player(dx, dy);
        moves++;
        render(moves);

        if (is_win()) {
			printf("\n");
            printf("Congratulations! Puzzle solved in %d moves.\n", moves);
            break;
        }
    }
    printf("您的操作为：");
    for(int j=0; j<strlen(mo); j++)
	{
		printf("%c", mo[j]);
	}
    return 0;
}

