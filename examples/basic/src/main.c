#include "jadeitite.h"
#include "jadeitite_term.h"
#include "font8x8.h"

void onAttach(int p_argc, char **p_argv);
void onDetach(int p_argc, char **p_argv);
void onUpdate(void);
void onKeyboardDown(unsigned char p_key, int p_x, int p_y);
void onKeyboardUp(unsigned char p_key, int p_x, int p_y);
void onResize(int p_width, int p_height);

static jdt_winProp_t s_winProp = (jdt_winProp_t) {160, 160, "BASE TEMPLATE", 1};

jdt_term_asset_t *l_asset;
jdt_term_t *l_term;

int jdt_setup(jdt_callbacks_t *p_callbacks, jdt_winProp_t *p_winProp, int p_argc, char *p_argv[]) {
  p_callbacks->onAttach = onAttach;
  p_callbacks->onDetach = onDetach;
  p_callbacks->onKeyboardDown = onKeyboardDown;
  p_callbacks->onKeyboardUp = onKeyboardUp;
  p_callbacks->onUpdate = onUpdate;
  p_callbacks->onResize = onResize;

  p_winProp->width = s_winProp.width;
  p_winProp->height = s_winProp.height;
  p_winProp->label = s_winProp.label;
  p_winProp->autoRefresh = s_winProp.autoRefresh;

  return 1;
}

void onAttach(int p_argc, char **p_argv) {
  s_jdt_clear_color_renderer.x = 32;
  s_jdt_clear_color_renderer.y = 32;
  s_jdt_clear_color_renderer.z = 32;

  // Example for writing data
  typedef struct {
    u8 num_a;
    u8 num_b;
  } test_data_t;
  test_data_t l_test_data_1 = (test_data_t) {16, 32};
  jdt_file_write("test_data.dat", &l_test_data_1, sizeof(test_data_t));
  test_data_t *read_data = (test_data_t *) jdt_file_read(
    "test_data.dat",
    sizeof(test_data_t));
  JDT_LOG_INFO("Test Data 1 [%d; %d]", read_data->num_a, read_data->num_b);
  free(read_data);

  jdt_term_asset_create("font8x8.jta", 128, 8, font8x8_basic);

  l_asset = jdt_term_asset_init("font8x8.jta", 128, 8);

  l_term = jdt_term_init(s_winProp, (vec_2_u32){8, 8});
  l_term->asset = l_asset;

  l_term->color_draw = (jdt_term_cell_color_t){255, 0, 0};

  l_term->buffer.data[0].color = (jdt_term_cell_color_t){255, 0, 0};
  l_term->buffer.data[0].cell_id = 'a';

  onUpdate();
}

void onKeyboardDown(unsigned char p_key, int p_x, int p_y) {
  switch (p_key) {
  default:{
    JDT_LOG_DEBUG("Missing bind p_key on KeyboardDown for \"%c\"!", p_key);
    jdt_term_put(l_term, p_key);
    break;
  }
  }

  onUpdate(); // only if auto refresh is disabled
}

void onKeyboardUp(unsigned char p_key, int p_x, int p_y) {
  switch (p_key) {
  default:JDT_LOG_DEBUG("Missing bind p_key on KeyboardUp for \"%c\"!", p_key);
    break;
  }

//  onUpdate(); // only if auto refresh is disabled
}

u32 s_text_pos_x = 0;
bool s_text_pos_x_increasing = true;
u32 l_random_index = 0;

void onUpdate(void) {
//  if (s_text_pos_x >= 32) {
//    s_text_pos_x_increasing = false;
//  } else if (s_text_pos_x == 0) {
//    s_text_pos_x_increasing = true;
//  }
//
//  if (s_text_pos_x_increasing) {
//    s_text_pos_x++;
//  } else {
//    s_text_pos_x--;
//  }

  u8 l_rand = (u8)jdt_math_squirrel3(l_random_index, 123);
  if (l_rand > 'z') {
    l_rand = 'z';
  } else if (l_rand < 33) {
    l_rand = 33;
  }
  l_random_index++;
  jdt_term_put(l_term, l_rand);

  jdt_render_begin();

//  jdt_set_render_color(255, 0, 0);
//  jdt_draw_bitmaps(8, 8, l_asset->data, 32 + s_text_pos_x, 32, "Hello, World!");

  jdt_term_render(l_term);

  // Render things here
//  render_color(255, 0, 0);
//  draw_point_fast_l1_int(5, 5);
//  draw_circle_filled_int(25, 25, 8, 8);
//  draw_circle_int(50, 50, 16, 32);

  jdt_render_end();
}

void onDetach(int p_argc, char **p_argv) {
  JDT_LOG_INFO("Detaching...");
  jdt_term_destroy(l_term);
}

void onResize(int p_width, int p_height) {
//  if (p_width > 0 && p_height > 0) {
//    JDT_LOG_INFO("Resizing display [width=%d;height=%d]", p_width, p_height);
//    window_resize(p_width, p_height);
//  }
}
