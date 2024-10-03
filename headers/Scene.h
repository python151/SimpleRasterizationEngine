#ifndef B4C99C2D_6485_49E3_BD9F_9832BAA92C80
#define B4C99C2D_6485_49E3_BD9F_9832BAA92C80

#include "STL_Handler.h"
#include "stdlib.h"
#include "Lighting.h"

typedef struct {
    Vertex3D* location;
    double rotation[3];
    Model* model;
} GameObject;

typedef struct {
    Vertex3D* point;
    double rotation[3];
    double fov;
} Camera;

typedef struct {
    Camera* camera;
    GameObject** objects;
    size_t num_models;
    LightSource** light_sources;
    size_t num_lights;
} Scene;

typedef struct {
    int w, h;
    double* z;
} ZBuffer;

typedef struct {
    Vertex2D** vertices;  // Array of vertices
    size_t vertex_count;  // Number of vertices
    Triangle** triangles;  // Array of triangles
    size_t triangle_count;  // Number of triangles
    ZBuffer* z_buffer;
} Image2D;

Scene* build_scene_struct(Camera* camera, GameObject** objects, size_t num_models, LightSource** light_sources, size_t num_lights);
void destroy_scene_recursively(Scene* scene);

Camera* build_camera_struct(Vertex3D* point, double x, double y, double z, double fov);
void destroy_camera_struct(Camera* camera);

Image2D* build_empty_image_struct(size_t triangle_count, size_t vertex_count, int h, int w);
void destoy_image_struct(Image2D* object);

GameObject* build_gameobject(Vertex3D* point, Model* model);
void destroy_gameobject_struct(GameObject* object);

#endif /* B4C99C2D_6485_49E3_BD9F_9832BAA92C80 */
