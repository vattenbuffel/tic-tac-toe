#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "tic_tac_toe.h"
#include "assert_.h"
#include "min_max.h"


int main(int argc) {
    // Do basic checks
    assert_(!WON_BOARD(EMPTY_BOARD), "Should not win\n");
    assert_(WON_BOARD(0b111000000), "Should win row 2\n");
    assert_(WON_BOARD(0b100100100), "Should win col 2\n");
    assert_(WON_BOARD(0b100010001), "Should win diag 1\n");
    assert_(1 == ADD_PLACEMENT_TO_BOARD(0,0,EMPTY_BOARD), "Should be 1 with new placement\n");
    assert_(1 == EXTRACT_CELL_VAL(0,0,DIAG1_WIN_BOARD), "cell (0,0) should have value 1 in DIAG1WINBOARD\n");
    PRINT_BOARD(DIAG0_WIN_BOARD, DIAG1_WIN_BOARD);
    CALC_ENTRIES_BOARD(xs, 0b111111111);
    assert_(xs == 9, "There should be 9 xs");

    board_state_t *state = build_tree(0b000000011, 0b110000000, false);
    int16_t placement_bitmask = best_move_bit_mask(state);
    destroy_tree(state);
    assert_(placement_bitmask == 0b001000000, "Should be: 0b000000100\n");

    state = build_tree(0b110000000, 0b000000011, false);
    placement_bitmask = best_move_bit_mask(state);
    destroy_tree(state);
    assert_(placement_bitmask == 0b000000100, "Should be: 0b000000100\n");

    state = build_tree(0b010000010, 0b000010001, false);
    placement_bitmask = best_move_bit_mask(state);
    destroy_tree(state);
    assert_(placement_bitmask == 0b100000000, "Should be: 0b100000000\n");

    state = build_tree(0b000000000, 0b000000000, false);
    placement_bitmask = best_move_bit_mask(state);
    destroy_tree(state);
    
    state = build_tree(0b000000011, 0b100000000, false);
    placement_bitmask = best_move_bit_mask(state);
    destroy_tree(state);
    assert_(placement_bitmask == 0b000000100, "Should block x from winning");
    
    printf("Passed tests\n");

    // for (size_t i = 0; i < 1000; i++){
    //     board_state_t *state;
    //     state = build_tree(0b0, 0b0, false);
    //     destroy_tree(state);
    // }
    

    play();


    return 0;
}
