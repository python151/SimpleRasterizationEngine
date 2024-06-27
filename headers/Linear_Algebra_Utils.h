#ifndef AC8B4628_E080_4E41_8F65_BEC0FA0E92F0
#define AC8B4628_E080_4E41_8F65_BEC0FA0E92F0

#define VECTOR1D(values, length) \
    {                             \
        1,                        \
        (int[]) { length },       \
        values                    \
    }

typedef struct {
    int n_dimensions;
    int* shape;
    double* values;
} Matrix;


Matrix* construct_matrix(int n_dimensions, int* shape, double* values);
void destroy_matrix(Matrix* mat);

// Multiplies matrices
Matrix matmul(const Matrix mat1, const Matrix mat2);
void matmul_inplace(Matrix* result, const Matrix mat1, const Matrix mat2); 

// Matrix transposes
Matrix transpose(Matrix mat1);
void transpose_inplace(Matrix* mat1);

#endif /* AC8B4628_E080_4E41_8F65_BEC0FA0E92F0 */
