/**
 * (c) Ivan Janevski
 *
 * Matrix
 */

#include "common.h"

#define STRUCT_MATRIX_AT(row, col, height, width) \
    ((row) * (width) + (col))

// define: typedef
#define DEFINE_STRUCT_MATRIX_TYPEDEF(NAME, T) \
    typedef struct NAME NAME##_t;

// define: struct
#define DEFINE_STRUCT_MATRIX_STRUCT(NAME, T) \
    struct NAME { \
        T* m_data; \
        int m_width; \
        int m_height; \
        STRUCT_MUTEX_DECLARE(m_mutex) \
        STRUCT_MT_SAFE_DECLARE(m_mt_safe) \
    };

// define: new
#define DEFINE_STRUCT_MATRIX_NEW(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_new(int _height, int _width) \
    { \
        struct NAME *_matrix = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        int _i, _size = _width * _height; \
        _matrix->m_data = (T*) STRUCT_CALLOC(_size, T); \
        _matrix->m_width = _width; \
        _matrix->m_height = _height; \
        for (_i = 0; _i < _size; _i++) { \
            STRUCT_ASSIGN(_matrix->m_data[_i], STRUCT_ZERO); \
        } \
        STRUCT_MUTEX_INIT(_matrix->m_mutex) \
        STRUCT_MT_SAFE_INIT(_matrix->m_mt_safe) \
        return _matrix; \
    }

// define: init
#define DEFINE_STRUCT_MATRIX_INIT(NAME, T) \
    STRUCT_ATTRIB void NAME##_init(struct NAME *_matrix, int _height, int _width) \
    { \
        int _i, _size = _width * _height; \
        _matrix->m_data = (T*) STRUCT_CALLOC(_size, T); \
        _matrix->m_width = _width; \
        _matrix->m_height = _height; \
        for (_i = 0; _i < _size; _i++) { \
            STRUCT_ASSIGN(_matrix->m_data[_i], STRUCT_ZERO); \
        } \
        STRUCT_MUTEX_INIT(_matrix->m_mutex) \
        STRUCT_MT_SAFE_INIT(_matrix->m_mt_safe) \
    }

// define: destroy
#define DEFINE_STRUCT_MATRIX_DESTROY(NAME, T) \
    STRUCT_ATTRIB void NAME##_destroy(struct NAME *_matrix) \
    { \
        STRUCT_FREE(_matrix->m_data); \
        _matrix->m_data = STRUCT_NULL; \
        _matrix->m_width = 0; \
        _matrix->m_height = 0; \
        STRUCT_MUTEX_DESTROY(_matrix->m_mutex) \
        STRUCT_MT_SAFE_DESTROY(_matrix->m_mt_safe) \
    }

// define: free
#define DEFINE_STRUCT_MATRIX_FREE(NAME, T) \
    STRUCT_ATTRIB void NAME##_free(struct NAME *_matrix) \
    { \
        STRUCT_FREE(_matrix->m_data); \
        _matrix->m_data = STRUCT_NULL; \
        _matrix->m_width = 0; \
        _matrix->m_height = 0; \
        STRUCT_MUTEX_DESTROY(_matrix->m_mutex) \
        STRUCT_MT_SAFE_DESTROY(_matrix->m_mt_safe) \
        STRUCT_FREE(_matrix); \
    }

// define: width
#define DEFINE_STRUCT_MATRIX_WIDTH(NAME, T) \
    STRUCT_ATTRIB int NAME##_width(struct NAME *_matrix) \
    { \
        STRUCT_MT_SAFE_LOCK(_matrix->m_mt_safe) \
        int _width = _matrix->m_width; \
        STRUCT_MT_SAFE_UNLOCK(_matrix->m_mt_safe) \
        return _width; \
    }

// define: height 
#define DEFINE_STRUCT_MATRIX_HEIGHT(NAME, T) \
    STRUCT_ATTRIB int NAME##_height(struct NAME *_matrix) \
    { \
        STRUCT_MT_SAFE_LOCK(_matrix->m_mt_safe) \
        int _height  = _matrix->m_height; \
        STRUCT_MT_SAFE_UNLOCK(_matrix->m_mt_safe) \
        return _height; \
    }

// define: clone
#define DEFINE_STRUCT_MATRIX_CLONE(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_clone(struct NAME *_matrix)\
    { \
        STRUCT_MT_SAFE_LOCK(_matrix->m_mt_safe) \
        struct NAME *_clone = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        int _i, _size = _matrix->m_width * _matrix->m_height; \
        _clone->m_data = (T*) STRUCT_CALLOC(_size, T); \
        _clone->m_width = _matrix->m_width; \
        _clone->m_height = _matrix->m_height; \
        for (_i = 0; _i < _size; _i++) { \
            STRUCT_ASSIGN(_clone->m_data[_i], _matrix->m_data[_i]); \
        } \
        STRUCT_MUTEX_INIT(_clone->m_mutex) \
        STRUCT_MT_SAFE_INIT(_clone->m_mt_safe) \
        STRUCT_MT_SAFE_UNLOCK(_matrix->m_mt_safe) \
        return _clone; \
    }

// define: submatrix
#define DEFINE_STRUCT_MATRIX_CLONE(NAME, T) \
    STRUCT_ATTRIB struct NAME *NAME##_submatrix(struct NAME *_matrix, int _row, int _col, int _height, int _width)\
    { \
        STRUCT_MT_SAFE_LOCK(_matrix->m_mt_safe) \
        struct NAME *_clone = (struct NAME*) STRUCT_CALLOC(1, struct NAME); \
        int _s_row, _s_col, _s_width = _width, _s_height = _height; \
        if (_width < 0 || _col + _width > _matrix->m_width) \
            _s_width = _matrix->m_width; \
        if (_height < 0 || _row + _height > _matrix->m_height) \
            _s_height = _matrix->m_height; \
        _clone->m_data = (T*) STRUCT_CALLOC(_s_width * _s_height, T); \
        _clone->m_width = _s_width; \
        _clone->m_height = _s_height; \
        for (_s_row = 0; _s_row < _s_height; _row++) { \
            for (_s_col = 0; _s_col < _s_width; _s_col++) { \
                STRUCT_ASSIGN( \
                    _clone->m_data[STRUCT_MATRIX_AT(_s_row, _s_col, _s_height, _s_width)],  \
                    _matrix->m_data[STRUCT_MATRIX_AT(_row +  _s_row, _col + _s_col, _matrix->m_height, _matrix->m_width)] \
                ); \
            } \
        } \
        STRUCT_MUTEX_INIT(_clone->m_mutex) \
        STRUCT_MT_SAFE_INIT(_clone->m_mt_safe) \
        STRUCT_MT_SAFE_UNLOCK(_matrix->m_mt_safe) \
        return _clone; \
    }

// TODO:
// define: matrix
#define DEFINE_STRUCT_MATRIX(NAME, T) \
    DEFINE_STRUCT_MATRIX_TYPEDEF(NAME, T) \
    DEFINE_STRUCT_MATRIX_STRUCT(NAME, T) \
    DEFINE_STRUCT_MATRIX_NEW(NAME, T) \
    DEFINE_STRUCT_MATRIX_INIT(NAME, T) \
    DEFINE_STRUCT_MATRIX_DESTROY(NAME, T) \
    DEFINE_STRUCT_MATRIX_FREE(NAME, T) \
    DEFINE_STRUCT_MATRIX_WIDTH(NAME, T) \
    DEFINE_STRUCT_MATRIX_HEIGHT(NAME, T) \
    DEFINE_STRUCT_MATRIX_CLONE(NAME, T) \
    DEFINE_STRUCT_MATRIX_SUBMATRIX(NAME, T) \
    \
    DEFINE_STRUCT_MATRIX_LOCK(NAME, T) \
    DEFINE_STRUCT_MATRIX_UNLOCK(NAME, T) \
    DEFINE_STRUCT_MATRIX_MT_SAFE_LOCK(NAME, T) \
    DEFINE_STRUCT_MATRIX_MT_SAFE_UNLOCK(NAME, T) \
    \
    DEFINE_STRUCT_MATRIX_IS_DIAGONAL(NAME, T) \
    DEFINE_STRUCT_MATRIX_IS_TRIANGULAR(NAME, T) \
    DEFINE_STRUCT_MATRIX_IS_UPPER_TRIANGULAR(NAME, T) \
    DEFINE_STRUCT_MATRIX_IS_LOWER_TRIANGULAR(NAME, T) \
    DEFINE_STRUCT_MATRIX_IS_EQUAL(NAME, T) \
    DEFINE_STRUCT_MATRIX_IS_VECTOR(NAME, T) \
    DEFINE_STRUCT_MATRIX_IS_ROW_VECTOR(NAME, T) \
    DEFINE_STRUCT_MATRIX_IS_COL_VECTOR(NAME, T) \
    \
    DEFINE_STRUCT_MATRIX_FILL(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_IDENTITY(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_ZERO_MATRIX(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_SCALING(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_TRANSLATION(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_ROTATION(NAME, T) \
    \
    DEFINE_STRUCT_MATRIX_SET_MATRIX(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_MATRIX(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_MATRIX(NAME, T) \
    DEFINE_STRUCT_MATRIX_TRANSPOSE(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADJUGATE(NAME, T) \
    DEFINE_STRUCT_MATRIX_INVERSE(NAME, T) \
    DEFINE_STRUCT_MATRIX_CONJUGATE(NAME, T) \
    DEFINE_STRUCT_MATRIX_TRACE(NAME, T) \
    \
    DEFINE_STRUCT_MATRIX_PREPEND_ROW(NAME, T) \
    DEFINE_STRUCT_MATRIX_PREPEND_COL(NAME, T) \
    DEFINE_STRUCT_MATRIX_APPEND_ROW(NAME, T) \
    DEFINE_STRUCT_MATRIX_APPEND_COL(NAME, T) \
    DEFINE_STRUCT_MATRIX_INSERT_ROW(NAME, T) \
    DEFINE_STRUCT_MATRIX_INSERT_COL(NAME, T) \
    DEFINE_STRUCT_MATRIX_DELETE_ROW(NAME, T) \
    DEFINE_STRUCT_MATRIX_DELETE_COL(NAME, T) \
    DEFINE_STRUCT_MATRIX_SWAP_ROW(NAME, T) \
    DEFINE_STRUCT_MATRIX_SWAP_COL(NAME, T) \
    \
    DEFINE_STRUCT_MATRIX_SET(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_ROW(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_COL(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_DIAG(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_MAIN_DIAG(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_ANTI_DIAG(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_TRIANGLE(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_UPPER_TRIANGLE(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_LOWER_TRIANGLE(NAME, T) \
    DEFINE_STRUCT_MATRIX_SET_ALL(NAME, T) \
    DEFINE_STRUCT_MATRIX_ROW_SET_ROW(NAME, T)\
    DEFINE_STRUCT_MATRIX_COL_SET_COL(NAME, T)\
    \
    DEFINE_STRUCT_MATRIX_ADD(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_ROW(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_COL(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_DIAG(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_MAIN_DIAG(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_ANTI_DIAG(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_TRIANGLE(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_UPPER_TRIANGLE(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_LOWER_TRIANGLE(NAME, T) \
    DEFINE_STRUCT_MATRIX_ADD_ALL(NAME, T) \
    DEFINE_STRUCT_MATRIX_ROW_ADD_ROW(NAME, T)\
    DEFINE_STRUCT_MATRIX_COL_ADD_COL(NAME, T)\
    \
    DEFINE_STRUCT_MATRIX_MUL(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_ROW(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_COL(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_DIAG(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_MAIN_DIAG(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_ANTI_DIAG(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_TRIANGLE(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_UPPER_TRIANGLE(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_LOWER_TRIANGLE(NAME, T) \
    DEFINE_STRUCT_MATRIX_MUL_ALL(NAME, T) \
    DEFINE_STRUCT_MATRIX_ROW_MUL_ROW(NAME, T)\
    DEFINE_STRUCT_MATRIX_COL_MUL_COL(NAME, T)\
    \
    DEFINE_STRUCT_MATRIX_DETERMINANT(NAME, T) \
    DEFINE_STRUCT_MATRIX_COFACTOR(NAME, T) \
    DEFINE_STRUCT_MATRIX_GAUSS(NAME, T) \
    DEFINE_STRUCT_MATRIX_LU(NAME, T) \
    \
    DEFINE_STRUCT_MATRIX_EXP(NAME, T) \
    DEFINE_STRUCT_MATRIX_SIN(NAME, T) \
    DEFINE_STRUCT_MATRIX_COS(NAME, T) \
    DEFINE_STRUCT_MATRIX_LOG(NAME, T) \

