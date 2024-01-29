#include <stdio.h>
#include <struct/matrix.h>

DEFINE_STRUCT_MATRIX(matrix, int);

void matrix_print(matrix_t *mat)
{
    int row, col;
    for (row = 0; row < matrix_height(mat); row++) {
        for (col = 0; col < matrix_width(mat); col++) {
            printf("%d ", matrix_get(mat, row, col));
        }
        printf("\n");
    }
}

int main() {
    // Mat 1
    matrix_t *mat1 = matrix_new(4, 4);
    matrix_set(mat1, 1, 2, 2);
    printf("Mat1: \n");
    matrix_print(mat1);

    // Mat 2
    matrix_t *mat2 = matrix_new(4, 4);
    matrix_set(mat2, 1, 2, 2);

    // Tests
    printf("Is mat1 Diagonal? %s\n", matrix_is_diagonal(mat1) ? "Yes" : "No");
    printf("Is mat1 Lower triangualr? %s\n", matrix_is_lower_triangular(mat1) ? "Yes" : "No");
    printf("Is mat1 Upper triangualr? %s\n", matrix_is_upper_triangular(mat1) ? "Yes" : "No");
    printf("Is mat1 == mat2? %s\n", matrix_equals(mat1, mat2) ? "Yes" : "No");
    printf("Is mat1 square? %s\n", matrix_is_square(mat1) ? "Yes" : "No");

    // Free memory
    matrix_free(mat1);
    matrix_free(mat2);
    return 0;
}

