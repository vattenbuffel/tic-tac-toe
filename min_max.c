#include "min_max.h"
#include "timing.h"
#include "tic_tac_toe.h"
#include <stdint.h>
#include "assert_.h"
#include "math_.h"

static void eval_state(board_state_t *state, int8_t depth) {
  if (WON_BOARD(state->board_x)) {
    state->value = SCORE_LOSE;
  } else if (WON_BOARD(state->board_o)) {
    state->value = SCORE_WIN - depth ;
  } else {
    state->value = 0;
  }
  return;
}

static int8_t min_max(board_state_t *state, bool maximizing_player, int8_t depth) {
  // printf("Evaluating board: \n");
  // PRINT_BOARD(state->board_x, state->board_o);
  if (state->done || depth == DEPTH_MAX) {
    eval_state(state, depth);
    // PRINT_BOARD_VALUE(state);
    return state->value;
  }

    int8_t value;
    int8_t i = 0;
    if (maximizing_player){
        state->value = INT8_MIN;
        while(state->children[i] != NULL){
            value = min_max(state->children[i], false, depth+1);
            state->value = MAX(state->value, value);
            i++;
        }
        return state->value;
    }
    else{
        state->value = INT8_MAX;
        while(state->children[i] != NULL){
            value = min_max(state->children[i], true, depth+1);
            state->value = MIN(state->value, value);
            i++;
        }
        return state->value;
    }
}

int16_t best_move_bit_mask(board_state_t *state) {
  // Find the best possible value of the current state
  TIMING_START;
  int8_t val = min_max(state, true, 0);
  double ms = TIMING_END_MS;
  printf("It took %f ms to find best move\n", ms);
  // printf("Best value: %d\n", (int)val);

  // Figure out which board the best value corresponds to
  board_state_t *best_state = NULL;
  for(int i = 0; i < CHILDREN_MAX; i++){
    if(state->children[i] == NULL)
      break;
    
    if (state->children[i]->value == val)
      best_state = state->children[i];
  }
  // printf("best nest board:\n");
  // PRINT_BOARD_VALUE(best_state);

  // Figure out what move to do to get to the best possible board
  int16_t bitmask_placement;
  if (state->player_x){
    bitmask_placement = best_state->board_x ^ state->board_x;
  }
  else{
    bitmask_placement = best_state->board_o ^ state->board_o;
  }

  CHECK_BIT_SUM(bitmask_placement);
  return bitmask_placement;
}
