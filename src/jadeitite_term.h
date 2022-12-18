#ifndef JADEITITE_TERM_H
#define JADEITITE_TERM_H

#include "jadeitite.h"

// =====================================================================================================================
//                                            Terminal structure
// =====================================================================================================================
// Terminal structures, initializer and destroyers
// ---------------------------------------------------------------------------------------------------------------------

typedef struct {
  vec_3_u8 color_foreground;
  vec_3_u8 color_background;
} jdt_term_cell_color_t;

typedef struct {
  u32 cell_id; // pointer to asset/font
  jdt_term_cell_color_t color;
} jdt_term_cell_t;

typedef struct {
  jdt_term_cell_t *data;
  vec_3_u32 size; // z = x * y
} jdt_term_buffer_t;

typedef struct jdt_term_rt {
  jdt_term_buffer_t buffer;
  jdt_term_cell_color_t color_draw;
  jdt_term_cell_color_t color_clear;
  vec_2_u32 cursor;
} jdt_term_t;

static jdt_term_t *jdt_term_init(vec_2_u32 p_term_size) {
  jdt_term_t *l_term = malloc(sizeof(jdt_term_t));

  l_term->buffer.size = (vec_3_u32){p_term_size.x, p_term_size.y, p_term_size.x * p_term_size.y};
  l_term->buffer.data = malloc(sizeof(jdt_term_buffer_t) * l_term->buffer.size.z);

  return l_term;
}

static void jdt_term_destroy(jdt_term_t *p_term) {
    free(p_term->buffer.data);
    free(p_term);
}

// =====================================================================================================================
//                                            Terminal operators
// =====================================================================================================================
// Terminal operators such like renderer and buffer operation
// ---------------------------------------------------------------------------------------------------------------------

static void jdt_term_set_color_draw(jdt_term_t *p_term, jdt_term_cell_color_t p_color_draw) {
  p_term->color_draw = p_color_draw;
}

static void jdt_term_set_color_clear(jdt_term_t *p_term, jdt_term_cell_color_t p_color_clear) {
  p_term->color_clear = p_color_clear;
}

static void jdt_term_move_cursor(jdt_term_t *p_term) {
  if (p_term->cursor.x >= p_term->buffer.size.x - 1) {
    if (p_term->cursor.y >= p_term->buffer.size.y - 1) {
      // Move content
      // TODO
    } else {
      p_term->cursor.y++;
    }
    p_term->cursor.x = 0;
  } else {
    p_term->cursor.x++;
  }
}

static u8 jdt_term_put_at(jdt_term_t *p_term, u32 p_id, vec_2_u32 p_position) {
  // Check for overflow
  if (p_position.x >= p_term->buffer.size.x || p_position.y >= p_term->buffer.size.y) {
    JDT_LOG_ERROR(
      "Cannot put glyph outside of buffer due to overflow [post=[%d;%d]; size=[%d;%d]]!",
      p_position.x, p_position.y,
      p_term->buffer.size.x, p_term->buffer.size.y
      );
    return JDT_RS_FAILED;
  }

  const size_t l_pos = p_position.x + (p_position.y * p_term->buffer.size.x);

  p_term->buffer.data[l_pos].cell_id = p_id;
  p_term->buffer.data[l_pos].color = p_term->color_draw;

  return JDT_RS_OK;
}

static void jdt_term_put(jdt_term_t *p_term, u32 p_id) {
  const size_t l_pos = p_term->cursor.x + (p_term->cursor.y * p_term->buffer.size.x);

  p_term->buffer.data[l_pos].cell_id = p_id;
  p_term->buffer.data[l_pos].color = p_term->color_draw;

  jdt_term_move_cursor(p_term);
}

#endif //JADEITITE_TERM_H
