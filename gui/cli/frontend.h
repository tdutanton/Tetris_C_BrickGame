/**
 * \file
 * \brief Header file for Tetris frontend
 *
 * Contains a description of the main data types and functions used for
 * presentation game in console screen
 */

#ifndef TETRIS_FRONTEND
#define TETRIS_FRONTEND

#include "../../brick_game/tetris/backend.h"

#define COLS_FRONT 20

typedef WINDOW* WIN;

void paint_boxes(void);
void print_stats(GameInfo_t info);
void print_greeting(void);
void fill_main_field(GameInfo_t info);
void fill_next_figure(GameInfo_t info);
void refresh_windows(void);
void print_current_game_state(GameInfo_t info);
void print_game_over(full_game_info_t full_info);
void print_pause(void);
void print_terminate(full_game_info_t full_info);

#endif