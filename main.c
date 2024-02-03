#define STRUCT_MT_SAFE
#include <math.h>
#include <stdio.h>
#include <struct/matrix.h>

DEFINE_STRUCT_MATRIX(mat, double)

void mat_print(mat_t *mat)
{
    int row, col;
    for (row = 0; row < mat_height(mat); row++) {
        for (col = 0; col < mat_width(mat); col++) {
            printf("%g ", mat_get(mat, row, col));
        }
        printf("\n");
    }
}

mat_t *mat_rot_new(double a)
{
    /*
      cos(a) -sin(a) 0
      sin(a) cos(a)  0
        0      0     1
    */
    mat_t *mat = mat_new(3, 3);
    double c = cos(a);
    double s = sin(a);
    mat_zero(mat);
    mat_set(mat, 0, 0, +c);
    mat_set(mat, 1, 1, +c);
    mat_set(mat, 0, 1, -s);
    mat_set(mat, 1, 0, +s);
    mat_set(mat, 2, 2, 1.);
    return mat;
}

mat_t *mat_rot_x_new(double a)
{
    /*
      cos(a) -sin(a) 0 0
      sin(a) cos(a)  0 0
        0      0     1 0
        0      0     0 1
    */
    mat_t *mat = mat_new(4, 4);
    double c = cos(a);
    double s = sin(a);
    mat_zero(mat);
    mat_set(mat, 0, 0, +c);
    mat_set(mat, 1, 1, +c);
    mat_set(mat, 0, 1, -s);
    mat_set(mat, 1, 0, +s);
    mat_set(mat, 2, 2, 1.);
    mat_set(mat, 3, 3, 1.);
    return mat;
}

int main() {
    // Mat 1
    printf("\nMat1: \n");
    mat_t *mat1 = mat_new(4, 4);
    mat_zero(mat1);
    mat_identity(mat1);
    mat_set_main_diagonal(mat1, 9);
    mat_print(mat1);

    // Rot mat
    printf("\nRot: \n");
    mat_t *rot = mat_rot_x_new(M_PI / 2.);
    mat_print(rot);

    // Mat 2
    printf("\nMat2: \n");
    mat_t *mat2 = mat_new(4, 1);
    mat_set(mat2, 0, 0, 4);
    mat_lmul(mat2, rot);
    mat_print(mat2);

    //printf("\nMat3: \n");
    //mat_t *mat3 = mat_clone(mat2);
    //mat_set_identity(mat1);
    //mat_set_identity(mat2);
    //mat_set_zero(mat3);
    //mat_mul_matrix(mat3, mat1, mat2);
    //mat_set_row(mat3, 0, 3);
    //mat_set_col(mat3, 3, 3);
    //mat_set_col(mat3, 0, 3);
    //mat_set_row(mat3, 3, 3);
    //mat_set_main_diagonal(mat3, 5);
    //mat_set_anti_diagonal(mat3, 4);
    //mat_set_upper_triangle(mat3, 9);
    //mat_set_lower_triangle(mat3, 8);
    //mat_set_all(mat3, 0);
    //mat_col_set_col(mat3, 0, 2);
    //mat_add_col(mat3, 0, 9);
    //mat_col_add_col(mat3, 0, 2);
    //mat_transpose(mat3);
    //mat_transpose(mat3);
    //mat_print(mat3);
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(0 * 4, 255); }
    { void * x = (void*) calloc(0 * 4, 255); }
    { void * x = (void*) calloc(0 * 4, 255); }
    { void * x = (void*) calloc(0 * 4, 255); }
    { void * x = (void*) calloc(0 * 4, 255); }
    { void * x = (void*) calloc(0 * 4, 255); }
    { void * x = (void*) calloc(0 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }
    { void * x = (void*) calloc(10 * 4, 255); }


    // Tests
    printf("\nTests: \n");
    printf("Is mat1 Diagonal? %s\n", mat_is_diagonal(mat1) ? "Yes" : "No");
    printf("Is mat1 Lower triangualr? %s\n", mat_is_lower_triangle(mat1) ? "Yes" : "No");
    printf("Is mat1 Upper triangualr? %s\n", mat_is_upper_triangle(mat1) ? "Yes" : "No");
    printf("Is mat1 == mat2? %s\n", mat_equals(mat1, mat2) ? "Yes" : "No");
    printf("Is mat1 square? %s\n", mat_is_square(mat1) ? "Yes" : "No");
    printf("Is mat1 row vector? %s\n", mat_is_row_vector(mat1) ? "Yes" : "No");
    printf("Is mat1 col vector? %s\n", mat_is_col_vector(mat1) ? "Yes" : "No");
    printf("Is mat1 vector? %s\n", mat_is_vector(mat1) ? "Yes" : "No");

    // Free memory
    mat_free(mat1);
    mat_free(mat2);
    return 0;
}

