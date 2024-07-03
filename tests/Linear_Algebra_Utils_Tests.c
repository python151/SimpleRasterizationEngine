#include <stdio.h>
#include "../source/Linear_Algebra_Utils.c"


void PrintMatrix(Matrix* m) {
    puts("Printing matrix:");
    for (int i = 0; i < m->shape[0]; i++) {
        for (int j = 0; j < m->shape[1]; j++) {
            printf("%.2f ", m->values[i*m->shape[1] + j]);
        }
        puts(" ");
    }
}

int main(int argc, char* argv[]) {
    size_t* shape = malloc(2*sizeof(size_t));
    shape[0] = 2;
    shape[1] = 1;

    double* values = malloc(shape[0]*shape[1]*sizeof(double));
    values[0] = 1;
    values[1] = 2;
    Matrix* mat0 = construct_matrix(2, shape, values);

    size_t* shape1 = malloc(2*sizeof(size_t));
    shape1[0] = 2;
    shape1[1] = 2;
    double* values1 = malloc(shape[0]*shape[1]*sizeof(double));
    values1[0] = 1;
    values1[1] = 2;
    values1[2] = 3;
    values1[3] = 4;
    Matrix* mat1 = construct_matrix(2, shape1, values1);

    PrintMatrix(mat0);
    PrintMatrix(mat1);

    Matrix* mat2 = matmul(*mat1, *mat0);
    if (mat2 == NULL)
        puts("MAT2 IS NULL");
    printf("%ld", mat2->n_dimensions);
    PrintMatrix(mat2);

    scalar_multiply_inplace(mat2, 3);
    PrintMatrix(mat2);

    destroy_matrix(mat0);
    destroy_matrix(mat1);
    destroy_matrix(mat2);

    return 0;
}
