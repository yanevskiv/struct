#define STRUCT_MT_SAFE
#include <stdio.h>
#include <struct/matrix.h>

DEFINE_STRUCT_MATRIX(matrix, int)

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
    matrix_set(mat1, 0, 0, 2);
    matrix_set(mat1, 3, 0, 2);
    matrix_set_zero(mat1);
    matrix_set_identity(mat1);
    printf("Mat1: \n");
    matrix_print(mat1);

    // Mat 2
    printf("\nMat2: \n");
    matrix_t *mat2 = matrix_new(4, 4);
    matrix_set_matrix(mat2, mat1);
    matrix_add_matrix(mat2, mat2, mat2);
    matrix_add_matrix(mat2, mat2, mat2);
    matrix_add_matrix(mat2, mat2, mat2);
    matrix_print(mat2);

    printf("\nMat3: \n");
    matrix_t *mat3 = matrix_clone(mat2);
    matrix_set_identity(mat1);
    matrix_set_identity(mat2);
    matrix_set_zero(mat3);
    matrix_mul_matrix(mat3, mat1, mat2);
    matrix_set_row(mat3, 0, 3);
    matrix_set_col(mat3, 3, 3);
    matrix_set_col(mat3, 0, 3);
    matrix_set_row(mat3, 3, 3);
    matrix_set_main_diagonal(mat3, 5);
    matrix_set_anti_diagonal(mat3, 4);
    matrix_set_upper_triangle(mat3, 9);
    matrix_set_lower_triangle(mat3, 8);
    matrix_set_all(mat3, 0);
    matrix_set_col(mat3, 0, 3);
    matrix_col_set_col(mat3, 0, 2);
    matrix_print(mat3);


    // Tests
    printf("\nTests: \n");
    printf("Is mat1 Diagonal? %s\n", matrix_is_diagonal(mat1) ? "Yes" : "No");
    printf("Is mat1 Lower triangualr? %s\n", matrix_is_lower_triangle(mat1) ? "Yes" : "No");
    printf("Is mat1 Upper triangualr? %s\n", matrix_is_upper_triangle(mat1) ? "Yes" : "No");
    printf("Is mat1 == mat2? %s\n", matrix_equals(mat1, mat2) ? "Yes" : "No");
    printf("Is mat1 square? %s\n", matrix_is_square(mat1) ? "Yes" : "No");
    printf("Is mat1 row vector? %s\n", matrix_is_row_vector(mat1) ? "Yes" : "No");
    printf("Is mat1 col vector? %s\n", matrix_is_col_vector(mat1) ? "Yes" : "No");
    printf("Is mat1 vector? %s\n", matrix_is_vector(mat1) ? "Yes" : "No");

    // Free memory
    matrix_free(mat1);
    matrix_free(mat2);
    return 0;
}

