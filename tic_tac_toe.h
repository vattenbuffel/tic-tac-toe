#ifndef TIC_TAC_TOE_H_
#define TIC_TAC_TOE_H_


#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "assert_.h"
#include "math_.h"

// Fucky wucky macro to calculate how big an elment of a struct is
#define member_size(type, member) sizeof(((type *)0)->member)

#define CELL_TO_POS(x, y) ((y)*3 + (x))
#define EXTRACT_CELL_VAL(x, y, board) (((board) >> CELL_TO_POS((x), (y))) & 1)

// Macro to see if board done
#define DRAW_BOARD(board) ((board) == 0b111111111)
#define DONE_BOARD(board_x, board_o, board_combined) (DRAW_BOARD((board_combined)) | WON_BOARD((board_x)) | WON_BOARD((board_o)))

// Macros to calculate if player won, board: int16_t
#define WON_BOARD(board) (WON_VERT((board)) | WON_HOR((board)) | WON_DIAG((board)))

#define WON_VERT(board) (WON_VERT0((board)) | WON_VERT1((board)) | WON_VERT2((board)))
#define WON_VERT0(board) (((board) & VERT0_WIN_BOARD) == VERT0_WIN_BOARD)
#define WON_VERT1(board) (((board) & VERT1_WIN_BOARD) == VERT1_WIN_BOARD)
#define WON_VERT2(board) (((board) & VERT2_WIN_BOARD) == VERT2_WIN_BOARD)


#define WON_HOR(board) (WON_HOR0((board)) | WON_HOR1((board)) | WON_HOR2((board)))
#define WON_HOR0(board) (((board) & HOR0_WIN_BOARD) == HOR0_WIN_BOARD)
#define WON_HOR1(board) (((board) & HOR1_WIN_BOARD) == HOR1_WIN_BOARD)
#define WON_HOR2(board) (((board) & HOR2_WIN_BOARD) == HOR2_WIN_BOARD)


#define WON_DIAG(board) (WON_DIAG0((board)) | WON_DIAG1((board)))
#define WON_DIAG0(board) (((board) & DIAG0_WIN_BOARD) == DIAG0_WIN_BOARD)
#define WON_DIAG1(board) (((board) & DIAG1_WIN_BOARD) == DIAG1_WIN_BOARD)

// Vert 0 is col 0
#define VERT0_WIN_BOARD (0b001001001)
// Vert 1 is col 1
#define VERT1_WIN_BOARD (0b010010010)
// Vert 2 is col 2
#define VERT2_WIN_BOARD (0b100100100)

// HOR 0 is row 0
#define HOR0_WIN_BOARD (0b000000111)
// HOR 1 is row 1
#define HOR1_WIN_BOARD (0b000111000)
// HOR 2 is row 2
#define HOR2_WIN_BOARD (0b111000000)

// Diag 0 is diag from up left
#define DIAG0_WIN_BOARD (0b001010100)
// Diag 1 is diag from down left
#define DIAG1_WIN_BOARD (0b100010001)

#define EMPTY_BOARD 0
 
#define CHILDREN_MAX 9

// Macro to combine x-board and o-board to complete board
#define COMBINE_BOARDS(board_x, board_o) ((board_x) | (board_o))

#define VALID_PLACEMENT(x,y,board) (VALID_PLACEMENT_BIT_MASK(1 << CELL_TO_POS((x), (y)), (board)))

#define VALID_PLACEMENT_BIT_MASK(bit_mask, board) (((board) & (bit_mask)) == 0)

#define CHECK_BIT_SUM(bitmask){\
  int8_t bit_sum = 0; \
  for (int _i_ = 0; _i_ < 9; _i_++) \
    bit_sum += ((bitmask)>>_i_) & 1; \
  assert_(bit_sum == 1, "Only 1 cell can be placed at a time\n"); \
}

#define CALC_ENTRIES_BOARD(var_name, board)\
  int8_t var_name = 0; \
  for (int _i_ = 0; _i_ < 9; _i_++){ \
      var_name += ((board)>>_i_) & 1; \
    }
  
#define VALID_XS_OS(board_x, board_o){ \
    CALC_ENTRIES_BOARD(xs, board_x);\
    CALC_ENTRIES_BOARD(os, board_o);\
    assert_(ABS(xs-os) <= 1, "Number of xs and os must be equal or at most 1 more x or o.\n");\
  }


// Macro to add a placement in a cell to a board
#define ADD_PLACEMENT_TO_BOARD(x, y, board) ADD_PLACEMENT_TO_BOARD_BIT_MASK((board) , (1 << CELL_TO_POS((x), (y))))
#define ADD_PLACEMENT_TO_BOARD_BIT_MASK(bit_mask, board) ((board) | (bit_mask))

#define PRINT_CELL(x, y, board_x, board_o) \
    char c = (1 == EXTRACT_CELL_VAL((x), (y), (board_x))) ?  'x': ' '; \
    c = (1 == EXTRACT_CELL_VAL((x), (y), (board_o))) ? 'o' : c; \
    printf("| %c |", c);

#define PRINT_BOARD(board_x, board_o){ \
  printf("---------------\n");\
  for (int y = 2; y >= 0; y--) {\
    for (int x = 0; x < 3; x++) {\
      PRINT_CELL(x, y, (board_x), (board_o));\
    }\
  printf("\n---------------\n");\
  }\
}

#define FIRST_PLAYER_X true

typedef struct board_state{
    int16_t board_x;
    int16_t board_o;
    int16_t board_combined;
    struct board_state* parent;
    struct board_state* children[CHILDREN_MAX];
    bool done;
    int8_t value;
    bool player_x; // If it's player_x who made the move at this state
} board_state_t;

typedef struct{
    bool x_cur_player;
    int16_t board_x;
    int16_t board_o;
    int16_t board_combined;
}game_t;


board_state_t *build_tree(int16_t board_x, int16_t board_o, bool player_x);
void destroy_tree(board_state_t *board_state);
void play();

#endif /* TIC_TAC_TOE_H_ */