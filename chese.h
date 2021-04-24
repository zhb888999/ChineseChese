#ifndef CHESE_CHESE_H
#define CHESE_CHESE_H

#define CANVAS_SIZE 657
#define BOARD_SIZE 91

/* chese term */
#define RED  0
#define BLACK 8

/* chese type */
#define NONE 0b000      // 空
#define KING 0b001      // 帅
#define ADVISER 0b010   // 仕
#define ELEPHANT 0b011  // 象
#define HORSE 0b100     // 马
#define CHARIOT 0b101   // 车
#define CANNON 0b110    // 炮
#define PAWN 0b111      // 兵

/* move error code */
#define NO_ERROR 0
#define NO_CHESE 1
#define SAME_TEAM 2
#define TEAM_ERROR 3
#define OUT_OF_BOUNDS 4
#define GAME_OVER 5
#define RED_WIN 6
#define BLACK_WIN 7
#define BREAK_THE_RULE 8
#define SAME_POSITION 9

#define init_chese(TEAM, TYPE) (TEAM + TYPE)
#define get_team(CHESE) (CHESE & 0x8)
#define get_type(CHESE) (CHESE & 0x7)
#define get_state(STATE) (STATE & 0x1)

typedef unsigned char board;
typedef unsigned char chese;
typedef unsigned char team;
typedef unsigned char type;
typedef unsigned char state;
typedef char canvas;

board *create();

void destory(board *);

char *chese2srting(chese);

canvas *draw_board(const board *, canvas *);

void init(board *);

int move(unsigned int src, unsigned int dst, board *);

int play(unsigned int src_x, unsigned int src_y, unsigned int dst_x, unsigned int dst_y, board *);

int winner(const board *);

canvas *create_canvas();

void clear_canvas(canvas *);

void destory_canvas(canvas *);

#endif //CHESE_CHESE_H
