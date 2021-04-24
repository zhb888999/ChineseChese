#include <string.h>
#include <stdlib.h>
#include "chess.h"

void init(board *pboard) {
    int i, step;
    for (i = 0; i < BOARD_SIZE; i++) *(pboard + i) = 0;
    for (i = 0; i < 2; i++) {
        step = 8;
        *(pboard + i * step) = init_chess(RED, CHARIOT);
        *(pboard + 81 + i * step) = init_chess(BLACK, CHARIOT);

        step = 6;
        *(pboard + 1 + i * step) = init_chess(RED, HORSE);
        *(pboard + 82 + i * step) = init_chess(BLACK, HORSE);

        step = 4;
        *(pboard + 2 + i * step) = init_chess(RED, ELEPHANT);
        *(pboard + 83 + i * step) = init_chess(BLACK, ELEPHANT);

        step = 2;
        *(pboard + 3 + i * step) = init_chess(RED, ADVISER);
        *(pboard + 84 + i * step) = init_chess(BLACK, ADVISER);

        step = 6;
        *(pboard + 19 + i * step) = init_chess(RED, CANNON);
        *(pboard + 64 + i * step) = init_chess(BLACK, CANNON);
    }
    for (i = 0; i < 5; i++) {
        *(pboard + 27 + i * 2) = init_chess(RED, PAWN);
        *(pboard + 54 + i * 2) = init_chess(BLACK, PAWN);
    }
    *(pboard + 4) = init_chess(RED, KING);
    *(pboard + 85) = init_chess(BLACK, KING);
}

char *chess2string(chess ch) {
    switch (ch) {
        case 0b0001:
            return "帅";
        case 0b1001:
            return "将";
        case 0b0010:
            return "仕";
        case 0b1010:
            return "士";
        case 0b0011:
            return "相";
        case 0b1011:
            return "象";
        case 0b0100:
            return "马";
        case 0b1100:
            return "馬";
        case 0b0101:
            return "车";
        case 0b1101:
            return "車";
        case 0b0110:
            return "炮";
        case 0b1110:
            return "砲";
        case 0b0111:
            return "兵";
        case 0b1111:
            return "卒";
        default:
            return "--";
    }
}

canvas *draw_board(const board *pboard, canvas *dest) {
    char *cell1 = "|   |   |   | \\ | / |   |   |   |\n";
    char *cell2 = "|   |   |   | / | \\ |   |   |   |\n";
    char *cell3 = "|                               |\n";
    char *cell4 = "|   |   |   |   |   |   |   |   |\n";

    clear_canvas(dest);
    for (int j = 0; j < 10; j++) {
        for (int i = 0; i < 9; i++) {
            strcat(dest, chess2string(*(pboard + j * 9 + i)));
            if (i == 8) break;
            strcat(dest, "--");
        }
        strcat(dest, "\n");
        if (j == 0 || j == 7) strcat(dest, cell1);
        else if (j == 1 || j == 8) strcat(dest, cell2);
        else if (j == 4) strcat(dest, cell3);
        else if (j != 9) strcat(dest, cell4);
    }
    return dest;
}

board *create() {
    board *board = malloc(BOARD_SIZE);
    init(board);
    return board;
}

void destory(board *board) {
    free(board);
}

int check_chariot(unsigned int src, unsigned dst, const board *pboard) {
    unsigned int src_x, src_y, dst_x, dst_y, min, max;
    unsigned int step = 0;
    src_y = src / 9;
    src_x = src % 9;
    dst_y = dst / 9;
    dst_x = dst % 9;
    if (src_y == dst_y) step = 1;
    if (src_x == dst_x) step = 9;
    if (!step) return 0;
    if (src < dst) {
        min = src;
        max = dst;
    } else {
        min = dst;
        max = src;
    }
    for (;;) {
        min += step;
        if (min >= max) break;
        if (get_type(*(pboard + min)) != NONE) return 0;
    }
    return 1;
}

int check_adviser(unsigned int src, unsigned dst) {
    unsigned int dst_y, dst_x, dis;
    dst_y = dst / 9;
    dst_x = dst % 9;
    if (dst_x < 3 || dst_x > 5) return 0;
    if (dst_y > 2 && dst_y < 7) return 0;
    dis = src > dst ? src - dst : dst - src;
    if (dis == 8 || dis == 10) return 1;
    return 0;
}

int check_elephant(unsigned int src, unsigned dst, team src_team, const board *pboard) {
    unsigned int src_x, src_y, dst_x, dst_y, dis, x, y;
    src_y = src / 9;
    src_x = src % 9;
    dst_y = dst / 9;
    dst_x = dst % 9;
    if (src_team == RED && dst > 44) return 0;
    if (src_team == BLACK && dst < 45) return 0;
    dis = src > dst ? src - dst : dst - src;
    if (dis != 16 && dis != 20) return 0;
    x = src_x > dst_x ? src_x - 1 : src_x + 1;
    y = src_y > dst_y ? src_y - 1 : src_y + 1;
    if (*(pboard + x + y * 9) != NONE) return 0;
    return 1;
}

int check_horse(unsigned int src, unsigned dst, const board *pboard) {
    unsigned int src_x, src_y, dst_x, dst_y, sub_x, sub_y, adjacent;
    src_y = src / 9;
    src_x = src % 9;
    dst_y = dst / 9;
    dst_x = dst % 9;
    sub_x = src_x > dst_x ? src_x - dst_x : dst_x - src_x;
    sub_y = src_y > dst_y ? src_y - dst_y : dst_y - src_y;
    if (sub_x + sub_y != 3) return 0;
    if (sub_x == 2) {
        adjacent = src_x < dst_x ? src + 1 : src - 1;
    } else {
        adjacent = src_y < dst_y ? src + 9 : src - 9;
    }
    if (get_type(*(pboard + adjacent)) != NONE) return 0;
    return 1;
}

int check_king(unsigned int src, unsigned dst, type dst_type, const board *pboard) {
    unsigned int src_x, src_y, dst_x, dst_y, dis, min, max;
    dst_y = dst / 9;
    dst_x = dst % 9;
    if (dst_x < 3 || dst_x > 5) return 0;
    if (dst_y > 2 && dst_y < 7) return 0;
    src_y = src / 9;
    src_x = src % 9;
    if (src_y != dst_y && src_x != dst_x) return 0;
    dis = src > dst ? src - dst : dst - src;
    if (dst_type != KING) {
        if (dis != 1 && dis != 9) return 0;
    } else {
        if (src < dst) {
            min = src;
            max = dst;
        } else {
            min = dst;
            max = src;
        }
        for (;;) {
            min += 9;
            if (min >= max) break;
            if (get_type(*(pboard + min)) != NONE) return 0;
        }
    }
    return 1;
}

int check_cannon(unsigned int src, unsigned dst, type dst_type, const board *pboard) {
    unsigned int src_x, src_y, dst_x, dst_y, min, max, middel = 0;
    unsigned int step = 0;
    src_y = src / 9;
    src_x = src % 9;
    dst_y = dst / 9;
    dst_x = dst % 9;
    if (src_y == dst_y) step = 1;
    if (src_x == dst_x) step = 9;
    if (!step) return 0;
    if (src < dst) {
        min = src;
        max = dst;
    } else {
        min = dst;
        max = src;
    }
    for (;;) {
        min += step;
        if (min >= max) break;
        if (get_type(*(pboard + min)) != NONE) middel++;
    }
    if ((middel == 0 && dst_type == NONE) || middel == 1 && dst_type != NONE) return 1;
    return 0;
}

int check_pawn(unsigned int src, unsigned dst, team src_team) {
    unsigned int src_x, src_y, dst_x, dst_y, sub_x, sub_y;
    src_y = src / 9;
    src_x = src % 9;
    dst_y = dst / 9;
    dst_x = dst % 9;
    sub_x = src_x > dst_x ? src_x - dst_x : dst_x - src_x;
    sub_y = src_y > dst_y ? src_y - dst_y : dst_y - src_y;
    if (sub_x + sub_y != 1) return 0;
    if (src_team == RED) {
        if (dst_y < src_y) return 0;
        if (sub_x == 1 && dst < 45) return 0;
        return 1;
    }
    if (dst_y > src_y) return 0;
    if (sub_x == 1 && dst > 44) return 0;
    return 1;
}

int check_move(unsigned int src, unsigned dst, team src_team, type src_type, type dst_type, board *pboard) {
    switch (src_type) {
        case KING:
            return check_king(src, dst, dst_type, pboard);
        case ADVISER:
            return check_adviser(src, dst);
        case ELEPHANT:
            return check_elephant(src, dst, src_team, pboard);
        case HORSE:
            return check_horse(src, dst, pboard);
        case CHARIOT:
            return check_chariot(src, dst, pboard);
        case CANNON:
            return check_cannon(src, dst, dst_type, pboard);
        case PAWN:
            return check_pawn(src, dst, src_team);
        default:
            return 0;
    }
}

int move(unsigned int src, unsigned int dst, board *pboard) {
    if (get_state(*(pboard + 90))) return GAME_OVER;
    if (src > 89 || dst > 89) return OUT_OF_BOUNDS;
    if (src == dst) return SAME_POSITION;
    team cur_team = get_team(*(pboard + 90));
    team src_team = get_team(*(pboard + src));
    team dst_team = get_team(*(pboard + dst));
    type src_type = get_type(*(pboard + src));
    type dst_type = get_type(*(pboard + dst));

    if (src_type == NONE) return NO_CHESS;
    if (src_team != cur_team) return TEAM_ERROR;
    if (dst_type != NONE && src_team == dst_team) return SAME_TEAM;

    if (!check_move(src, dst, src_team, src_type, dst_type, pboard)) return BREAK_THE_RULE;

    *(pboard + dst) = *(pboard + src);
    *(pboard + src) = 0;

    if (dst_type == KING) {
        *(pboard + 90) += 1;
        return cur_team ? BLACK_WIN : RED_WIN;
    }
    *(pboard + 90) = cur_team ? RED : BLACK;
    return NO_ERROR;
}

int play(unsigned int src_x, unsigned int src_y, unsigned int dst_x, unsigned int dst_y, board *pboard)
{
    unsigned int src, dst;
    src = src_x + 9 * src_y;
    dst = dst_x + 9 * dst_y;
    return move(src, dst, pboard);
}

int winner(const board *pboard) {
    if (!get_state(*(pboard + 90))) return -1;
    return get_team(*(pboard + 90));
}

canvas *create_canvas() {
    return malloc(CANVAS_SIZE);
}

void clear_canvas(canvas *pcanvas) {
    for (int i = 0; i < CANVAS_SIZE; i++) *(pcanvas + i) = 0;
}

void destory_canvas(canvas *pcanvas) {
    free(pcanvas);
}