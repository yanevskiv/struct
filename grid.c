#include <grid.h>
#include <stdlib.h>

grid_t *grid_new(int width, int height)
{
    grid_t *grid = calloc(1, sizeof(grid_t));
    if (! grid)
        goto err;
    grid->g_width = width;
    grid->g_height = height;
    grid->g_data = calloc(width * height, sizeof(grid_data_t));
    return grid;
grid_err:
    free(grid);
    return NULL;
}

void grid_free(grid_t *grid)
{
    free(grid->g_data);
    free(grid);
}
