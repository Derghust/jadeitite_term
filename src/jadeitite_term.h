#ifndef JADEITITE_TERM_H
#define JADEITITE_TERM_H

#include "jadeitite.h"

// =====================================================================================================================
//                                                    Terminal
// =====================================================================================================================
// TODO
// ---------------------------------------------------------------------------------------------------------------------

typedef struct {
  u32 cell_id; // pointer to asset/font
  vec_3_u8 color_foreground;
  vec_3_u8 color_background;
} t_jdt_term_cell;

typedef struct {
  t_jdt_term_cell *data;
  vec_3_u32 size; // z = x * y
} jdt_term_buffer_t;

typedef struct jdt_term_rt {
  jdt_term_buffer_t buffer;
} jdt_term_t;

jdt_term_t *jdt_term_init(vec_2_u32 p_term_size) {
  jdt_term_t *l_term = malloc(sizeof(jdt_term_t));



  return l_term;
}

#endif //JADEITITE_TERM_H
