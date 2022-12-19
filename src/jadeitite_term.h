#ifndef JADEITITE_TERM_H
#define JADEITITE_TERM_H

#include "jadeitite.h"

// =====================================================================================================================
//                                            Terminal structure
// =====================================================================================================================
// Terminal structures
// ---------------------------------------------------------------------------------------------------------------------

typedef struct {
  vec_3_u32 size;
  jdt_winProp_t window;
} jdt_term_properties_t;

typedef struct {
  vec_3_u8 color_foreground;
  vec_3_u8 color_background;
} jdt_term_cell_color_t;

typedef struct {
  u8 cell_id; // pointer to asset/font
  jdt_term_cell_color_t color;
} jdt_term_cell_t;

typedef struct {
  jdt_term_cell_t *data;
} jdt_term_buffer_t;

typedef struct {
  u8 data[256][8];
} jdt_term_asset_t;

typedef struct {
  jdt_term_buffer_t buffer;
  jdt_term_cell_color_t color_draw;
  jdt_term_cell_color_t color_clear;
  vec_2_u32 cursor;
  jdt_term_asset_t *asset;
  jdt_term_properties_t properties;
} jdt_term_t;

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
  if (p_term->cursor.x >= p_term->properties.size.x - 1) {
    if (p_term->cursor.y >= p_term->properties.size.y - 1) {
      for (int y = 1; y < p_term->properties.size.y; ++y) {
        for (int x = 0; x < p_term->properties.size.x; ++x) {
          const size_t l_pos_written = x + ((y - 1) * p_term->properties.size.x);
          const size_t l_pos_moved = x + (y * p_term->properties.size.x);

          p_term->buffer.data[l_pos_written] = p_term->buffer.data[l_pos_moved];
        }
      }
      for (int x = 0; x < p_term->properties.size.x; ++x) {
        const size_t l_pos = x + ((p_term->properties.size.y - 1) * p_term->properties.size.x);

        p_term->buffer.data[l_pos].cell_id = 0;
      }
    } else {
      p_term->cursor.y++;
    }
    p_term->cursor.x = 0;
  } else {
    p_term->cursor.x++;
  }
}

static void jdt_term_clean(jdt_term_t *p_term) {
  for (int y = 0; y < p_term->properties.size.y; ++y) {
    for (int x = 0; x < p_term->properties.size.x; ++x) {
      const size_t l_pos = x + (y * p_term->properties.size.x);

      p_term->buffer.data[l_pos].cell_id = 0;

      jdt_set_render_color(
        p_term->color_clear.color_background.x,
        p_term->color_clear.color_background.y,
        p_term->color_clear.color_background.z
        );
      jdt_draw_rectangle_filled(x, y, 8, 8);
    }
  }

  for (int i = 0; i < p_term->properties.size.z; ++i) {

  }
}

static u8 jdt_term_set_cursor(jdt_term_t *p_term, vec_2_u32 p_position) {
  if (p_position.x >= p_term->properties.size.x - 1 || p_position.y >= p_term->properties.size.y - 1) {
    JDT_LOG_ERROR(
      "Cannot put glyph outside of buffer due to overflow [post=[%d;%d]; size=[%d;%d]]!",
      p_position.x, p_position.y,
      p_term->properties.size.x, p_term->properties.size.y
    );
    return JDT_RS_FAILED;
  } else {
    p_term->cursor = p_position;
  }
  return JDT_RS_OK;
}

static void jdt_term_put(jdt_term_t *p_term, u8 p_id) {
  const size_t l_pos = p_term->cursor.x + (p_term->cursor.y * p_term->properties.size.x);

  p_term->buffer.data[l_pos].cell_id = p_id;
  p_term->buffer.data[l_pos].color = p_term->color_draw;

  jdt_term_move_cursor(p_term);
}

static u8 jdt_term_put_at(jdt_term_t *p_term, u8 p_id, vec_2_u32 p_position) {
  if (jdt_term_set_cursor(p_term, p_position) != JDT_RS_OK) {
    jdt_term_put(p_term, p_id);
  } else {
    return JDT_RS_OK;
  }
}

// =====================================================================================================================
//                                       Terminal initializers and destructors
// =====================================================================================================================
// Terminal initializer and destructor functions for jdt_term structures
// ---------------------------------------------------------------------------------------------------------------------

static jdt_term_t *jdt_term_init(jdt_winProp_t p_properties, vec_2_u32 p_asset_size) {
  vec_2_u32 l_term_size = (vec_2_u32){p_properties.width / p_asset_size.x, p_properties.height / p_asset_size.y};

  jdt_term_t *l_term = malloc(sizeof(jdt_term_t));

  l_term->properties.size = (vec_3_u32){l_term_size.x, l_term_size.y, l_term_size.x * l_term_size.y};
  l_term->buffer.data = malloc(sizeof(jdt_term_buffer_t) * l_term->properties.size.z);

  l_term->color_clear.color_background = (vec_3_u8){0,0,0};

  l_term->cursor.x = 0;
  l_term->cursor.y = 0;

  jdt_term_clean(l_term);

  return l_term;
}

static void jdt_term_destroy(jdt_term_t *p_term) {
    free(p_term->buffer.data);
    free(p_term);
}

static jdt_term_asset_t *jdt_term_asset_init(char *p_path, size_t p_size, size_t p_height) {
  return jdt_file_read(p_path, sizeof(jdt_term_asset_t));
}

static u8 jdt_term_asset_create(char *p_path, size_t p_size, size_t p_height, const u8 p_bitmap[p_size][p_height]) {
  jdt_term_asset_t l_asset;
  for (int x = 0; x < p_size; ++x) {
    for (int y = 0; y < p_height; ++y) {
      l_asset.data[x][y] = p_bitmap[x][y];
    }
  }
  jdt_file_write(p_path, &l_asset, sizeof(jdt_term_asset_t));
}

// =====================================================================================================================
//                                            Terminal renderer
// =====================================================================================================================
// Terminal renderer
// ---------------------------------------------------------------------------------------------------------------------

static void jdt_term_render(jdt_term_t *p_term) {
  for (int y = 0; y < p_term->properties.size.y; ++y) {
    for (int x = 0; x < p_term->properties.size.x; ++x) {
      const size_t l_pos = x + (y * p_term->properties.size.x);
      const jdt_term_cell_t l_cell = p_term->buffer.data[l_pos];
      const jdt_term_cell_color_t l_color = l_cell.color;
      jdt_set_render_color(l_color.color_foreground.x, l_color.color_foreground.y, l_color.color_foreground.z);
      jdt_draw_bitmap(8, 8, p_term->asset->data[l_cell.cell_id], x * 8, y * 8);
    }
  }
}

#endif //JADEITITE_TERM_H
