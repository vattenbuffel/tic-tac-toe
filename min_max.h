#ifndef MIN_MAX_H_
#define MIN_MAX_H_

#include <stdint.h>
#include "tic_tac_toe.h"

#define SCORE_WIN 10
#define SCORE_LOSE -10

#define DEPTH_MAX 10

#define PRINT_BOARD_VALUE(board_state){\
    PRINT_BOARD(board_state->board_x, board_state->board_o);\
    printf("Board has value: %d\n", (int32_t) board_state->value);\
}

int16_t best_move_bit_mask(board_state_t *state);


#endif /* MIN_MAX_H_ */