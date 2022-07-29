#ifndef __GRID_H__
#define __GRID_H__
#define GRID_AT_THE_END -1

typedef void* grid_data_t;

struct grid {
    int g_width;
    int g_height;
    grid_data_t *g_data;
} grid_t;


grid_t *grid_new(int, int);
void grid_free(grid_t*);
void grid_add_row(grid_t*);
void grid_add_col(grid_t*);
void grid_del_row(grid_t*);
void grid_del_col(grid_t*);
void grid_set_row(grid_t*);
void grid_set_col(grid_t*);

void grid_set(grid_t*, int, int, grid_data_t);
grid_data_t grid_get(grid_t, int, int);


#endif
