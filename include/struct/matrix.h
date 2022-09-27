#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef double mat_data_t;

struct mat {
    size_t m_width;
    size_t m_height;
    mat_data_t *m_data;
} mat_t;


mat_t *mat_new(int, int);
void mat_free(mat_t*);

void mat_row_set();
void mat_col_set();
void mat_set();
void mat_fill();

void mat_diag_set();

mat_t *mat_trans();



#endif
