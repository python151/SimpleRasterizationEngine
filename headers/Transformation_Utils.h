#include "Linear_Algebra_Utils.h"
#include "STL_Handler.h"

#ifndef F9458B0A_5DF7_41F7_941A_06D333862CA9
#define F9458B0A_5DF7_41F7_941A_06D333862CA9

Matrix* construct_translation_matrix(double dx, double dy, double dz);
Matrix* construct_rotation_matrix(double theta_x, double theta_y, double theta_z);
Matrix* construct_affine_template_of_coordinate(Vertex3D v);
Vertex3D* strip_coords_from_matrix(Matrix* mat);

#endif /* F9458B0A_5DF7_41F7_941A_06D333862CA9 */
