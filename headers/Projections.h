#include "Scene.h"
#include "STL_Handler.h"
#include "Linear_Algebra_Utils.h"

#ifndef D6AAA3EC_7415_4B5E_A5FD_8CB24A5145D3
#define D6AAA3EC_7415_4B5E_A5FD_8CB24A5145D3

Matrix* generate_perspective_projection_matrix(double fov, int w, int h);
Image2D* construct_image_buffer(Scene* Scene, int w, int h);

#endif /* D6AAA3EC_7415_4B5E_A5FD_8CB24A5145D3 */
