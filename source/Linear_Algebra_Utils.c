#include "Linear_Algebra_Utils.h"
#include "stdio.h"

Matrix* construct_matrix(size_t n_dimensions, size_t* shape, double* values) {
    if (n_dimensions < 1)
        return NULL;

    Matrix* m = malloc(sizeof(Matrix));
    m->n_dimensions = n_dimensions;
    m->shape = shape;
    m->values = values;
    return m;
}
void destroy_matrix(Matrix* mat) {
    free(mat->shape);
    free(mat->values);
    free(mat);
}

// Multiplies matrices, only supports 2D
Matrix* matmul(Matrix mat1, Matrix mat2) {
    /*if (mat1.n_dimensions != 2 || mat2.n_dimensions != 2)
        return NULL;*/

    /*if (mat1.shape[1] != mat2.shape[0]) // Num rows of left (mat1.shape[0]) == num cols of right (mat2.shape[1])
        return NULL;*/
    
    size_t* shape = malloc(2*sizeof(size_t));
    shape[0] = mat1.shape[0];
    shape[1] = mat2.shape[1];

    double* values = malloc(shape[0]*shape[1]*sizeof(double));
    Matrix* result = construct_matrix(2, shape, values);

    matmul_inplace(result, mat1, mat2);

    return result;
}

void matmul_inplace(Matrix* result, Matrix mat1, Matrix mat2) {
    if (mat1.n_dimensions != 2 || mat2.n_dimensions != 2) {
        puts("Error in matmul_inplace, n_dimensions != 2");
        return;
    }
        

    if (mat1.shape[1] != mat2.shape[0]) { // Num rows of left (mat1.shape[0]) == num cols of right (mat2.shape[1])
        puts("Error in matmul_inplace, mat1.shape[1] != mat2.shape[0]");
        return;
    }
        
    
    if (result->shape[0] != mat1.shape[0] || result->shape[1] != mat2.shape[1]){
        puts("Error in matmul_inplace, result->shape[0] != mat1.shape[0] || result->shape[1] != mat2.shape[1]");
        return;
    }

    for (int row = 0; row < mat1.shape[0]; row++) { // For each row in mat1
        for (int col = 0; col < mat2.shape[1]; col++) { // For each col in mat2
            int target_index = row * mat2.shape[1] + col;
            result->values[target_index] = 0;

            for (int v = 0; v < mat1.shape[1]; v++) {
                int curr_mat1 = row*mat1.shape[1] + v;
                int curr_mat2 = (v*mat2.shape[1]) + col;
                result->values[target_index] += mat1.values[curr_mat1] * mat2.values[curr_mat2];
            }
        }
    }
}

void scalar_multiply_inplace(Matrix* mat, double s) {
    int size = 1;
    for (int i = 0; i < mat->n_dimensions; i++)
        size *= mat->shape[i];
    for (int i = 0; i < size; i++)
        mat->values[i] *= s;
}