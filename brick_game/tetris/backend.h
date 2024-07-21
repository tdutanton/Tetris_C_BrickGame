/**
 * \file
 * \brief Header file for Tetris backend
 *
 * Contains a description of the main data types and functions used
 */

#ifndef TETRIS_BACKEND
#define TETRIS_BACKEND

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define ROWS 20
#define COLS 10
#define MAX_FIG_ROWS 4
#define MAX_FIG_COLS 4
#define SPACEBAR 32
#define EMPTY_BLOCK 0
#define CURRENT_FIGURE_BLOCK 1
#define ATTACHED_BLOCK 2
#define START_SPEED 1400
#define LEVEL_STEP 600
#define SQUARE_FIG 3
#define START_X_POS_SPAWN 3
#define RANDOM_ID get_random_fig_id()
#define FULL_INFO full_game_info_t* full_info = update_ptr_full()
#define REFRESH_FIGURE         \
  copy_only_attached_blocks(); \
  place_fig_on_field();

typedef enum {
  good_move,
  collision_right,
  collision_left,
  collision_down,
  collision_up,
} possible_collision;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
} UserAction_t;

typedef enum {
  START_STATE,
  SPAWN_STATE,
  MOVING_STATE,
  SHIFTING_STATE,
  ATTACHING_STATE,
  GAME_OVER_STATE,
  PAUSE_STATE,
  TERMINATE_STATE,
} game_state;

typedef enum { horizontal = 1, vertical = -1 } figure_current_status;

typedef struct {
  int** field;
  int** next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct FIGURE {
  int fig_id;
  char fig_name;
  int rows;
  int cols;
  int x;
  int y;
  figure_current_status status;
  int fig[MAX_FIG_ROWS][MAX_FIG_COLS];
} FIGURE;

typedef struct full_game_info_t {
  GameInfo_t info;
  FIGURE figure;
  int next_fig_id;
  game_state state;
  long long timer;
  long long msec_speed;
} full_game_info_t;

GameInfo_t updateCurrentState(void);
UserAction_t user_push(wchar_t key);
full_game_info_t* update_ptr_full(void);
FIGURE random_fig_to_spawn(int id);
void userInput(UserAction_t action, bool hold);
void copy_area(int** current, int** new_area, int rows, int cols);
void copy_figure(int current_figure[MAX_FIG_ROWS][MAX_FIG_COLS],
                 int** new_area);
void userInput(UserAction_t action, bool hold);
void moving_actions(full_game_info_t* full_info, UserAction_t action);
void attaching_actions(full_game_info_t* full_info);
void start_actions(full_game_info_t* full_info, UserAction_t action);
void prepare_start_field(void);
void free_field_memory(void);
void reset_next_field(void);
void spawn_figure(void);
void step_left(void);
void step_right(void);
void step_down(void);
void place_fig_on_field(void);
void copy_only_attached_blocks(void);
void rotate(void);
void rotate_fig_I(void);
void rotate_fig_I_right(full_game_info_t* full_info);
void rotate_fig_I_left(full_game_info_t* full_info);
void rotate_fig_S_Z(void);
void rotate_fig_left(full_game_info_t* full_info);
void rotate_fig_right(full_game_info_t* full_info);
void fall_down(void);
void pause_process(full_game_info_t* full_info);
void attaching_figure(void);
void check_and_shift_filled_line(void);
void step_down_attached_blocks(int line);
void read_high_score(void);
void update_high_score(void);
void clean_field(void);
void set_speed(void);
void set_level(void);
long long update_time(void);
int** init_empty_area(int rows, int cols);
int get_random_fig_id(void);
int is_not_attached_on_top(full_game_info_t full_info);
int updated_score(int lines);
possible_collision collision_left_check(full_game_info_t full_info);
possible_collision collision_right_check(full_game_info_t full_info);
possible_collision collision_up_check(full_game_info_t full_info);
possible_collision collision_down_check(full_game_info_t full_info);

#endif