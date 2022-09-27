#include "defs.h"

#ifndef STRUCT_AT
#define STRUCT_AT(i, j, M, N) ((j) * (M) + i)
#endif

/* DECLARE */
#define DECLARE_STRUCT_MATRIX_STRUCT(NAME, T, M, N) \
    struct NAME;
#define DECLARE_STRUCT_MATRIX_TYPEDEF_STRUCT(NAME, T, M, N) \
    typedef struct NAME NAME##_t;
#define DECLARE_STRUCT_MATRIX_ALLOC(NAME, T, M, N) \
    struct NAME *NAME##_alloc();
#define DECLARE_STRUCT_MATRIX_FREE(NAME, T, M, N) \
    void NAME##_free();

#define DECLARE_STRUCT_MATRIX(NAME, T, M, N) \
    DECLARE_STRUCT_MATRIX_STRUCT_TYPEDEF(NAME, T, M, N) \

/* DEFINE */
#define DEFINE_STRUCT_MATRIX_STRUCT(NAME, T, M, N) \
    struct NAME { \
        T *data; \
    };
#define DEFINE_STRUCT_MATRIX_TYPEDEF_STRUCT(NAME, T, M, N) \
    typedef struct NAME { \
        T *data; \
    } NAME##_t;
#define DEFINE_STRUCT_MATRIX_NEW(NAME, T, M, N) \
    struct NAME *NAME##_new() \
    { \
        struct NAME *_struct = STRUCT_CALLOC(1, sizeof(struct NAME)); \
        _struct->data = STRUCT_CALLOC((M) * (N), sizeof(T)); \
        return _struct; \
    }
#define DEFINE_STRUCT_MATRIX_FREE(NAME, T, M, N) \
    void NAME##_free(struct NAME *_struct) \
    { \
        STRUCT_FREE(_struct->data); \
        STRUCT_FREE(_struct); \
    } 
#define DEFINE_STRUCT_MATRIX_WIDTH(NAME, T, M, N) \
    size_t NAME##_width()  \
    { \
        return (N); \
    }
#define DEFINE_STRUCT_MATRIX_HEIGHT(NAME, T, M, N) \
    size_t NAME##_height() \
    { \
        return (M); \
    }
#define DEFINE_STRUCT_MATRIX_GET(NAME, T, M, N) \
    T NAME##_get(struct NAME *_struct, size_t _i, size_t _j) \
    { \
        if (_i < 0 || _j < 0 || _i >= (M) || _j >= (N)) \
            return STRUCT_DEFAULT; \
        return _struct->data[STRUCT_AT(_i, _j, (M), (N))]; \
    }
#define DEFINE_STRUCT_MATRIX_SET(NAME, T, M, N) \
    void NAME##_set(struct NAME *_struct, size_t _i, size_t _j, T _data) \
    { \
        if (_i < 0 || _j < 0 || _i >= (M) || _j >= (N)) \
            return; \
        _struct->data[STRUCT_AT(_i, _j, (M), (N))] = _data; \
    }
#define DEFINE_STRUCT_MATRIX_ADD(NAME, T, M, N) \
    void NAME##_add(struct NAME *_struct, size_t _i, size_t _j, T _data) \
    { \
        if (_i < 0 || _j < 0 || _i >= (M) || _j >= (N)) \
            return; \
        _struct->data[STRUCT_AT(_i, _j, (M), (N))] = STRUCT_SUM(_struct->data[STRUCT_AT(_i, _j, (M), (N))], _data); \
    }
#define DEFINE_STRUCT_MATRIX_MUL(NAME, T, M, N) \
    void NAME##_mul(struct NAME *_struct, size_t _i, size_t _j, T _data) \
    { \
        if (_i < 0 || _j < 0 || _i >= (M) || _j >= (N)) \
            return; \
        _struct->data[STRUCT_AT(_i, _j, (M), (N))] = STRUCT_MUL(_struct->data[STRUCT_AT(_i, _j, (M), (N))], _data); \
    }
#define DEFINE_STRUCT_MATRIX_FULL_SET(NAME, T, M, N) \
    void NAME##_full_set(struct NAME *_struct, T _data)  \
    { \
        size_t _i = 0, _j = 0; \
        while (_j < (N)) { \
            _i = 0; \
            while (_i < (M)) { \
                _struct->data[STRUCT_AT(_i, _j, (M), (N))] = _data; \
                _i++; \
            } \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_FULL_ADD(NAME, T, M, N) \
    void NAME##_full_add(struct NAME *_struct, T _data)  \
    { \
        size_t _i = 0, _j = 0; \
        while (_j < (N)) { \
            _i = 0; \
            while (_i < (M)) { \
                _struct->data[STRUCT_AT(_i, _j, (M), (N))] = STRUCT_SUM(_struct->data[STRUCT_AT(_i, _j, (M), (N))], _data); \
                _i++; \
            } \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_FULL_MUL(NAME, T, M, N) \
    void NAME##_full_mul(struct NAME *_struct, T _data)  \
    { \
        size_t _i = 0, _j = 0; \
        while (_j < (N)) { \
            _i = 0; \
            while (_i < (M)) { \
                _struct->data[STRUCT_AT(_i, _j, (M), (N))] = STRUCT_MUL(_struct->data[STRUCT_AT(_i, _j, (M), (N))], _data); \
                _i++; \
            } \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_ROW_SET(NAME, T, M, N) \
    void NAME##_row_set(struct NAME *_struct, size_t _row, T _data)\
    { \
        size_t _j = 0; \
        while (_j < (N)) { \
            _struct->data[STRUCT_AT(_row, _j, (M), (N))] = _data; \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_ROW_ADD(NAME, T, M, N) \
    void NAME##_row_add(struct NAME *_struct, size_t _row, T _data)\
    { \
        size_t _j = 0; \
        while (_j < (N)) { \
            _struct->data[STRUCT_AT(_row, _j, (M), (N))] = STRUCT_SUM(_struct->data[STRUCT_AT(_row, _j, (M), (N))], _data); \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_ROW_MUL(NAME, T, M, N) \
    void NAME##_row_mul(struct NAME *_struct, size_t _row, T _data)\
    { \
        size_t _j = 0; \
        while (_j < (N)) { \
            _struct->data[STRUCT_AT(_row, _j, (M), (N))] = STRUCT_MUL(_struct->data[STRUCT_AT(_row, _j, (M), (N))], _data); \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_ROW_SET_ROW(NAME, T, M, N) \
    void NAME##_row_set_row(struct NAME *_struct, size_t _row1, size_t _row2)\
    { \
        size_t _j = 0; \
        while (_j < (N)) { \
            _struct->data[STRUCT_AT(_row1, _j, (M), (N))] = _struct->data[STRUCT_AT(_row2, _j, (M), (N))]; \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_ROW_ADD_ROW(NAME, T, M, N) \
    void NAME##_row_add_row(struct NAME *_struct, size_t _row1, size_t _row2)\
    { \
        size_t _j = 0; \
        while (_j < (N)) { \
            _struct->data[STRUCT_AT(_row1, _j, (M), (N))] = STRUCT_SUM(_struct->data[STRUCT_AT(_row1, _j, (M), (N))], _struct->data[STRUCT_AT(_row2, _j, (M), (N))]); \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_ROW_MUL_ROW(NAME, T, M, N) \
    void NAME##_row_mul_row(struct NAME *_struct, size_t _row1, size_t _row2)\
    { \
        size_t _j = 0; \
        while (_j < (N)) { \
            _struct->data[STRUCT_AT(_row1, _j, (M), (N))] = STRUCT_MUL(_struct->data[STRUCT_AT(_row1, _j, (M), (N))], _struct->data[STRUCT_AT(_row2, _j, (M), (N))]); \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_ROW_SWAP(NAME, T, M, N) \
    void NAME##_row_swap(struct NAME *_struct, size_t _row1, size_t _row2)\
    { \
        size_t _j = 0; \
        while (_j < (N)) { \
            T _data = _struct->data[STRUCT_AT(_row1, _j, (M), (N))]; \
            _struct->data[STRUCT_AT(_row1, _j, (M), (N))] = _struct->data[STRUCT_AT(_row2, _j, (M), (N))]; \
            _struct->data[STRUCT_AT(_row2, _j, (M), (N))] = _data; \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_COL_SET(NAME, T, M, N) \
    void NAME##_col_set(struct NAME *_struct, size_t _col, T _data)\
    { \
        size_t _i = 0; \
        while (_i < (M)) { \
            _struct->data[STRUCT_AT(_i, _col, (M), (N))] = _data; \
            _i++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_COL_ADD(NAME, T, M, N) \
    void NAME##_col_add(struct NAME *_struct, size_t _col, T _data)\
    { \
        size_t _i = 0; \
        while (_i < (M)) { \
            _struct->data[STRUCT_AT(_i, _col, (M), (N))] = STRUCT_SUM(_struct->data[STRUCT_AT(_i, _col, (M), (N))], _data); \
            _i++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_COL_MUL(NAME, T, M, N) \
    void NAME##_col_mul(struct NAME *_struct, size_t _col, T _data)\
    { \
        size_t _i = 0; \
        while (_i < (M)) { \
            _struct->data[STRUCT_AT(_i, _col, (M), (N))] = STRUCT_MUL(_struct->data[STRUCT_AT(_i, _col, (M), (N))], _data); \
            _i++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_COL_SET_COL(NAME, T, M, N) \
    void NAME##_col_set_col(struct NAME *_struct, size_t _col1, size_t _col2)\
    { \
        size_t _i = 0; \
        while (_i < (M)) { \
            _struct->data[STRUCT_AT(_i, _col1, (M), (N))] = _struct->data[STRUCT_AT(_i, _col2, (M), (N))]; \
            _i++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_COL_ADD_COL(NAME, T, M, N) \
    void NAME##_col_add_col(struct NAME *_struct, size_t _row1, size_t _row2)\
    { \
        size_t _i = 0; \
        while (_i < (M)) { \
            _struct->data[STRUCT_AT(_i, _col1, (M), (N))] = STRUCT_ADD(_struct->data[STRUCT_AT(_i, _col2, (M), (N))], _struct->data[STRUCT_AT(_i, _col2, (M), (N))]); \
            _i++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_COL_MUL_COL(NAME, T, M, N) \
    void NAME##_col_mul_col(struct NAME *_struct, size_t _row1, size_t _row2)\
    { \
        size_t _i = 0; \
        while (_i < (M)) { \
            _struct->data[STRUCT_AT(_i, _col1, (M), (N))] = STRUCT_ADD(_struct->data[STRUCT_AT(_i, _col2, (M), (N))], _struct->data[STRUCT_AT(_i, _col2, (M), (N))]); \
            _i++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_COL_SWAP(NAME, T, M, N) \
    void NAME##_col_swap(struct NAME *_struct, size_t _col1, size_t _col2)\
    { \
        size_t _i = 0; \
        while (_i < (M)) { \
            T _data = _struct->data[STRUCT_AT(_i, _col1, (M), (N))]; \
            _struct->data[STRUCT_AT(_i, _col1, (M), (N))] = _struct->data[STRUCT_AT(_i, _col2, (M), (N))]; \
            _struct->data[STRUCT_AT(_i, _col2, (M), (N))] = _data; \
            _i++; \
        } \
    }

#define DEFINE_STRUCT_MATRIX_DIAG_SET(NAME, T, M, N) \
    void NAME##_diag_set(struct NAME *_struct, T _data)\
    { \
        size_t _i = 0, _j = 0; \
        while (_i < (M) && _j < (N)) { \
            _struct->data[STRUCT_AT(_i, _j, (M), (N))] = _data; \
            _i++; \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_DIAG_ADD(NAME, T, M, N) \
    void NAME##_diag_add(struct NAME *_struct, T _data)\
    { \
        size_t _i = 0, _j = 0; \
        while (_i < (M) && _j < (N)) { \
            _struct->data[STRUCT_AT(_i, _j, (M), (N))] = STRUCT_SUM(_struct->data[STRUCT_AT(_i, _j, (M), (N))], _data); \
            _i++; \
            _j++; \
        } \
    }
#define DEFINE_STRUCT_MATRIX_DIAG_MUL(NAME, T, M, N) \
    void NAME##_diag_mul(struct NAME *_struct, T _data)\
    { \
        size_t _i = 0, _j = 0; \
        while (_i < (M) && _j < (N)) { \
            _struct->data[STRUCT_AT(_i, _j, (M), (N))] = STRUCT_MUL(_struct->data[STRUCT_AT(_i, _j, (M), (N))], _data); \
            _i++; \
            _j++; \
        } \
    }

#define DEFINE_STRUCT_MATRIX_ROW_SET_VA(NAME, T, M, N) \
    void NAME##_row_set_va(struct NAME *_struct, size_t _row, ...) \
    { \
        size_t _i = _row, _j = 0; \
        va_list _args; \
        va_start(_args, _row); \
        while (_j < (N)) { \
            _struct->data[STRUCT_AT(_i, _j, (M), (N))] = va_arg(_args, T); \
            _j++; \
        } \
        va_end(_args); \
    }

#define DEFINE_STRUCT_MATRIX_COL_SET_VA(NAME, T, M, N) \
    void NAME##_col_set_va(struct NAME *_struct, size_t _col, ...) \
    { \
        size_t _i = 0, _j = _col; \
        va_list _args; \
        va_start(_args, _col); \
        while (_i < (M)) { \
            _struct->data[STRUCT_AT(_i, _j, (M), (N))] = va_arg(_args, T); \
            _i++; \
        } \
        va_end(_args); \
    }
#define DEFINE_STRUCT_MATRIX_DIAG_SET_VA(NAME, T, M, N) \
    void NAME##_diag_set_va(struct NAME *_struct, ...) \
    { \
        size_t _i = 0, _j = 0; \
        va_list _args; \
        va_start(_args, _struct); \
        while (_i < (M) && _j < (N)) { \
            _struct->data[STRUCT_AT(_i, _j, (M), (N))] = va_arg(_args, T); \
            _i++; \
            _j++; \
        } \
        va_end(_args); \
    }
#define DEFINE_STRUCT_MATRIX_CLONE(NAME, T, M, N) \
    struct NAME *NAME##_clone(struct NAME *_struct) \
    { \
        struct NAME *_new = STRUCT_CALLOC(1, sizeof(struct NAME)); \
        size_t _i = 0, _j = 0; \
        while (_j < (N))  { \
            _i = 0; \
            while (_i < (M)) { \
                _new->data[STRUCT_AT(_i, _j, (M), (N))] = _struct->data[STRUCT_AT(_i, _j, (M), (N))]; \
                _i++; \
            } \
            _j++; \
        } \
        return _new; \
    }
#define DEFINE_STRUCT_MATRIX_MATRIX_ADD(NAME, T, M, N) \
    void NAME##_matrix_add(struct NAME *_struct1, struct NAME *_struct2) \
    { \
        size_t _i = 0, _j = 0; \
        while (_j < (N))  { \
            _i = 0; \
            while (_i < (M)) { \
                _struct1->data[STRUCT_AT(_i, _j, (M), (N))] = STRUCT_SUM((_struct1->data[STRUCT_AT(_i, _j, (M), (N))]), (_struct2->data[STRUCT_AT(_i, _j, (M), (N))])); \
                _i++; \
            } \
            _j++; \
        } \
    }

#define DEFINE_STRUCT_MATRIX_TRANSPOSE(NAME1, NAME2) \
    void NAME1##_transpose(struct NAME1 *_struct1, struct NAME2 *_struct2) \
    { \
        size_t _i, _j, _M1, _N1, _M2, _N2; \
        _M1 = NAME1##_height(); \
        _N1 = NAME1##_width(); \
        _M2 = NAME2##_height(); \
        _N2 = NAME2##_width(); \
        for (_i = 0; _i < _M1; _i++) { \
            for (_j = 0; _j < _N1; _j++) { \
                _struct2->data[STRUCT_AT(_j, _i, _M2, _N2)] = _struct1->data[STRUCT_AT(_i, _j, _M1, _N1)]; \
            } \
        } \
    }

#define DEFINE_STRUCT_MATRIX_MULTIPLICATION(FUNCNAME, NAME1, NAME2, NAME3) \
    void NAME1##_##FUNCNAME(struct NAME1 *_struct1, struct NAME2 *_struct2, struct NAME3 *_struct3) \
    { \
        size_t _i, _j, _k, _M1, _N1, _M2, _N2, _M3, _N3; \
        _M1 = NAME1##_height();\
        _N1 = NAME1##_width();\
        _M2 = NAME2##_height();\
        _N2 = NAME2##_width();\
        _M3 = NAME3##_height();\
        _N3 = NAME3##_width();\
        for (_i = 0; _i < _M1; _i++)  { \
            for (_j = 0; _j < _N1; _j++) { \
                _struct1->data[STRUCT_AT(_i, _j, _M1, _N1)] = 0;\
                for (_k = 0; _k < _M2; _k++) { \
                    _struct1->data[STRUCT_AT(_i, _j, _M1, _N1)] = STRUCT_SUM( \
                        _struct1->data[STRUCT_AT(_i, _j, _M1, _N1)], \
                        STRUCT_MUL( \
                            _struct2->data[STRUCT_AT(_i, _k, _M2, _N2)],  \
                            _struct3->data[STRUCT_AT(_k, _j, _M3, _N3)] \
                        ) \
                    ); \
                } \
            } \
        } \
    }


#define DEFINE_STRUCT_MATRIX(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_TYPEDEF_STRUCT(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_NEW(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_FREE(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_CLONE(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_WIDTH(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_HEIGHT(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_GET(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_SET(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_ADD(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_MUL(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_FULL_SET(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_FULL_ADD(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_FULL_MUL(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_ROW_SET(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_ROW_ADD(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_ROW_MUL(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_COL_SET(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_COL_ADD(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_COL_MUL(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_DIAG_SET(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_DIAG_ADD(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_DIAG_MUL(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_ROW_SET_ROW(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_ROW_ADD_ROW(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_ROW_MUL_ROW(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_ROW_SWAP(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_COL_SWAP(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_MATRIX_ADD(NAME, T, M, N) \

    //DEFINE_STRUCT_MATRIX_ROW_SET_VA(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_COL_SET_VA(NAME, T, M, N) \
    DEFINE_STRUCT_MATRIX_DIAG_SET_VA(NAME, T, M, N) \

