#include "tic_tac_toe.h"
#include "timing.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "min_max.h"
#include <assert.h>
#include <string.h>

game_t game;

void build_tree_rec(board_state_t *parent, bool player_x, int32_t *states_cnt) {
  // Create children
  int8_t children_i = 0;
  bool valid_placement = false;
  for (int i = 0; i < 9; i++) {
    if (VALID_PLACEMENT_BIT_MASK(parent->board_combined, 1 << i)) {
      (*states_cnt)++;
      board_state_t *child = malloc(sizeof(board_state_t));

      parent->children[children_i] = child;
      if (player_x) {
        child->board_x = ADD_PLACEMENT_TO_BOARD_BIT_MASK(1 << i, parent->board_x);
        child->board_o = parent->board_o;
      } else {
        child->board_o = ADD_PLACEMENT_TO_BOARD_BIT_MASK(1 << i, parent->board_o);
        child->board_x = parent->board_x;
      }
      child->board_combined = COMBINE_BOARDS(child->board_x, child->board_o);

      memset(child->children, 0, member_size(board_state_t, children));
      child->parent = parent;
      child->done = DONE_BOARD(child->board_x, child->board_o, child->board_combined);
      child->value = 0;
      child->player_x = player_x;
      
      // Error checking
      // CHECK_BIT_SUM(child->board_combined ^ parent->board_combined);
      // VALID_XS_OS(child->board_x, child->board_o);
      // assert(child->board_combined >> 15 == 0);
      // assert(child->board_combined >> 14 == 0);
      // assert(child->board_combined >> 13 == 0);
      // assert(child->board_combined >> 12 == 0);
      // assert(child->board_combined >> 11 == 0);
      // assert(child->board_combined >> 10 == 0);
      // assert(child->board_combined >> 9 == 0);
      // assert(child->board_x >> 15 == 0);
      // assert(child->board_x >> 14 == 0);
      // assert(child->board_x >> 13 == 0);
      // assert(child->board_x >> 12 == 0);
      // assert(child->board_x >> 11 == 0);
      // assert(child->board_x >> 10 == 0);
      // assert(child->board_x >> 9 == 0);
      // assert(child->board_o >> 15 == 0);
      // assert(child->board_o >> 14 == 0);
      // assert(child->board_o >> 13 == 0);
      // assert(child->board_o >> 12 == 0);
      // assert(child->board_o >> 11 == 0);
      // assert(child->board_o >> 10 == 0);
      // assert(child->board_o >> 9 == 0);

      children_i++;
    }
    for (size_t i = 0; i < CHILDREN_MAX; i++){
      if (parent->children[i] == NULL)
        break;
      assert(parent->children[i]->parent == parent);
    }
    
  }

  // Create grand children
  for (; children_i > 0; children_i--){
    if (!parent->children[children_i-1]->done){
      build_tree_rec(parent->children[children_i - 1], !player_x, states_cnt);
      }
    }
}

board_state_t* build_tree(int16_t board_x, int16_t board_o, bool player_x) {
  board_state_t* parent_p = malloc(sizeof(board_state_t));
  parent_p->board_x = board_x;
  parent_p->board_o = board_o;
  parent_p->board_combined = COMBINE_BOARDS(board_x, board_o);
  parent_p->parent = NULL;
  memset(parent_p->children, 0, member_size(board_state_t, children));
  parent_p->player_x = player_x;
  parent_p->done = DONE_BOARD(parent_p->board_x, parent_p->board_o, parent_p->board_combined);
 
  // Error checking
  // VALID_XS_OS(parent_p->board_x, parent_p->board_o);
  int32_t states_cnt = 0;

  TIMING_START;
  build_tree_rec(parent_p, player_x, &states_cnt);
  double ms = TIMING_END_MS;
  printf("Created: %d boards. It took: %f ms\n", states_cnt, ms);
  return parent_p;
}

int16_t get_placement_bitmask_player(){
  int16_t placement_bitmask = 0;
  bool valid_bitmask;
  do {
    int x, y;
    printf("Where do you want to place (x y):\n");
    scanf("%d %d", &x, &y);
    valid_bitmask = VALID_PLACEMENT(x, y, game.board_combined) && 0 <= x && 2 >= x && 0 <= y && 2 >= y;
    placement_bitmask = 1 << CELL_TO_POS(x, y);

    if (!valid_bitmask){
      printf("Invalid placement.\n");
    }
  }while (!valid_bitmask);
  return placement_bitmask;
}

void play(){
  game.board_combined = EMPTY_BOARD;
  game.board_x = EMPTY_BOARD;
  game.board_o = EMPTY_BOARD;
  game.x_cur_player = FIRST_PLAYER_X;

  int16_t placement_bitmask;
  while(!DONE_BOARD(game.board_o, game.board_x, game.board_combined)){
    printf("Current board: \n");
    PRINT_BOARD(game.board_x, game.board_o);
    
    // Get next move
    if (game.x_cur_player){
      placement_bitmask = get_placement_bitmask_player();
    }
    else{
      board_state_t *state = build_tree(game.board_x, game.board_o, game.x_cur_player);
      placement_bitmask = best_move_bit_mask(state);
      destroy_tree(state);
    }

    // Update game state
    if (game.x_cur_player){
      game.board_x = ADD_PLACEMENT_TO_BOARD_BIT_MASK(placement_bitmask, game.board_x);
    }
    else {
      game.board_o = ADD_PLACEMENT_TO_BOARD_BIT_MASK(placement_bitmask, game.board_o);
    }
    game.board_combined = COMBINE_BOARDS(game.board_x, game.board_o);
    game.x_cur_player = !game.x_cur_player;


  }

  // Print final board
  printf("Current board: \n");
  PRINT_BOARD(game.board_x, game.board_o);

  // Check for winner
  if (WON_BOARD(game.board_x)){
    printf("Congratz, you won!\n");
    return;
  }
  else if (WON_BOARD(game.board_o)){
    printf("You lost\n");
    return;
  }
  else if (DRAW_BOARD(game.board_combined)){
    printf("It's a draw!\n");
    return;
  }



}

void destroy_tree_children(board_state_t *board_state, int8_t depth){
  // assert_(depth<10, "There can only be 9 moves at most so the tree cannot be deeper than 9 children.");
  // VALID_XS_OS(board_state->board_x, board_state->board_o);
  // assert(board_state->board_combined >> 15 == 0);
  // assert(board_state->board_combined >> 14 == 0);
  // assert(board_state->board_combined >> 13 == 0);
  // assert(board_state->board_combined >> 12 == 0);
  // assert(board_state->board_combined >> 11 == 0);
  // assert(board_state->board_combined >> 10 == 0);
  // assert(board_state->board_combined >> 9 == 0);
  // assert(board_state->board_x >> 15 == 0);
  // assert(board_state->board_x >> 14 == 0);
  // assert(board_state->board_x >> 13 == 0);
  // assert(board_state->board_x >> 12 == 0);
  // assert(board_state->board_x >> 11 == 0);
  // assert(board_state->board_x >> 10 == 0);
  // assert(board_state->board_x >> 9 == 0);
  // assert(board_state->board_o >> 15 == 0);
  // assert(board_state->board_o >> 14 == 0);
  // assert(board_state->board_o >> 13 == 0);
  // assert(board_state->board_o >> 12 == 0);
  // assert(board_state->board_o >> 11 == 0);
  // assert(board_state->board_o >> 10 == 0);
  // assert(board_state->board_o >> 9 == 0);

  for (size_t i = 0; i < CHILDREN_MAX; i++){
    if (board_state->children[i] == NULL)
      break;
    assert_(board_state->children[i]->parent == board_state, "The child's parent must be the child's parent");
    destroy_tree_children(board_state->children[i], depth+1);

  }
  // printf("Freeing: 0x%X\n", board_state);
  free(board_state);
}

void destroy_tree(board_state_t *board_state){
  destroy_tree_children(board_state, 0);
}
