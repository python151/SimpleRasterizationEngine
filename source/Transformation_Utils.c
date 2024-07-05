#include "Transformation_Utils.h"
#include <math.h>

Matrix* construct_translation_matrix(double dx, double dy, double dz) {
    size_t* shape = malloc(2*sizeof(size_t));
    shape[0] = 4;
    shape[1] = 4;
    double* values = malloc(16*sizeof(double));
    values[0] = 1;
    values[1] = 0;
    values[2] = 0;
    values[3] = dx;

    values[4] = 0;
    values[5] = 1;
    values[6] = 0;
    values[7] = dy;

    values[8] = 0;
    values[9] = 0;
    values[10] = 1;
    values[11] = dz;

    values[12] = 0;
    values[13] = 0;
    values[14] = 0;
    values[15] = 1;
    Matrix* m = construct_matrix(2, shape, values);
    return m;
}
Matrix* construct_rotation_matrix(double theta_x, double theta_y, double theta_z) {
    // TODO: IMPLEMENT THIS, THIS IS JUST A PLACEHOLDER
    Matrix* mat_x =  construct_translation_matrix(0, 0, 0);
    mat_x->values[5] = cos(theta_x);
    mat_x->values[6] = sin(theta_x);
    mat_x->values[9] = -sin(theta_x);
    mat_x->values[10] = cos(theta_x);

    Matrix* mat_y =  construct_translation_matrix(0, 0, 0);
    mat_y->values[0] = cos(theta_y);
    mat_y->values[2] = -sin(theta_y);
    mat_y->values[8] = sin(theta_y);
    mat_y->values[10] = cos(theta_y);

    Matrix* mat_z =  construct_translation_matrix(0, 0, 0);
    mat_z->values[0] = cos(theta_z);
    mat_z->values[1] = sin(theta_z);
    mat_z->values[4] = -sin(theta_z);
    mat_z->values[5] = cos(theta_z);

    Matrix* YZ = matmul(*mat_y, *mat_z);
    Matrix* XYZ = matmul(*mat_x, *YZ);

    destroy_matrix(mat_x);
    destroy_matrix(mat_y);
    destroy_matrix(mat_z);
    destroy_matrix(YZ);

    return XYZ;
}

Matrix* construct_affine_template_of_coordinate(Vertex3D v) {
    size_t* shape = malloc(2*sizeof(size_t));
    shape[0] = 4;
    shape[1] = 1;
    double* values = malloc(4*sizeof(double));
    values[0] = v.x; 
    values[1] = v.y; 
    values[2] = v.z; 
    values[3] = 1; 
    Matrix* m = construct_matrix(2, shape, values);
    return m;
}
Vertex3D* strip_coords_from_matrix(Matrix* mat) {
    Vertex3D* v = build_vertex3D_struct(mat->values[0], mat->values[1], mat->values[2]);
    return v;
}