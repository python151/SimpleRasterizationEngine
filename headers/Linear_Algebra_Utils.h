#include "stdlib.h"

#ifndef AC8B4628_E080_4E41_8F65_BEC0FA0E92F0
#define AC8B4628_E080_4E41_8F65_BEC0FA0E92F0

typedef struct {
    size_t n_dimensions;
    size_t* shape;
    double* values;
} Matrix;


Matrix* construct_matrix(size_t n_dimensions, size_t* shape, double* values);
void destroy_matrix(Matrix* mat);

// Multiplies matrices
Matrix* matmul(const Matrix mat1, const Matrix mat2);
void matmul_inplace(Matrix* result, const Matrix mat1, const Matrix mat2); 
void scalar_multiply_inplace(Matrix* mat, double s);

#endif /* AC8B4628_E080_4E41_8F65_BEC0FA0E92F0 */
