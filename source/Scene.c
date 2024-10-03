#include "Scene.h"

Scene* build_scene_struct(Camera* camera, GameObject** objects, size_t num_models, LightSource** light_sources, size_t num_lights) {
    Scene* scene = malloc(sizeof(Scene));
    scene->camera = camera;
    scene->objects = objects;
    scene->num_models = num_models;
    scene->light_sources = light_sources;
    scene->num_lights = num_lights;
    return scene;
}
void destroy_scene_recursively(Scene* scene) {
    destroy_camera_struct(scene->camera);
    for (int m = 0; m < scene->num_models; m++) {
        destroy_gameobject_struct(scene->objects[m]);
    }
    for (int l = 0; l < scene->num_lights; l++) {
        destroy_l
    }
    free(scene->objects);
    free(scene);
}

Camera* build_camera_struct(Vertex3D* point, double x, double y, double z, double fov) {
    Camera* camera = malloc(sizeof(Camera));
    camera->point = point;
    camera->fov = fov;
    camera->rotation[0] = x;
    camera->rotation[1] = y;
    camera->rotation[2] = z;
    return camera;
}
void destroy_camera_struct(Camera* camera) {
    destroy_vertex3D_struct(camera->point);
    free(camera);
}

ZBuffer* create_empty_z_buffer(int h, int w) {
    ZBuffer* z_buffer = malloc(sizeof(ZBuffer));
    z_buffer->h = h;
    z_buffer->w = w;
    z_buffer->z = malloc(h*w*sizeof(double));
}
void destroy_z_buffer(ZBuffer* object) {
    free(object->z);
    free(object);
}

Image2D* build_empty_image_struct(size_t triangle_count, size_t vertex_count, int h, int w) {
    Image2D* image = malloc(sizeof(Image2D));
    image->triangle_count = triangle_count;
    image->vertex_count = vertex_count;
    printf("T count is %zu and V count is %zu\n", image->triangle_count, image->vertex_count);
    image->triangles = malloc(triangle_count*sizeof(Triangle*));
    image->vertices = malloc(vertex_count*sizeof(Vertex2D*));
    if (image->vertices == NULL)
        puts("ITS ALL A LIE");
    else   
        puts("ITS ALL TRUE!");
    image->z_buffer = create_empty_z_buffer(h, w);
    return image;
}
void destoy_image_struct(Image2D* object) {
    destroy_z_buffer(object->z_buffer);
    for (int t = 0; t < object->triangle_count; t++)
        destroy_triangle_struct(object->triangles[t]);
    for (int v = 0; v < object->vertex_count; v++)
        destroy_vertex2D_struct(object->vertices[v]);
    free(object->triangles);
    free(object->vertices);
    free(object);
}

GameObject* build_gameobject(Vertex3D* point, Model* model) {
    GameObject* object = malloc(sizeof(GameObject));
    object->location = point;
    object->model = model;
    object->rotation[0] = 0;
    object->rotation[1] = 0;
    object->rotation[2] = 0;
    return object;
}
void destroy_gameobject_struct(GameObject* object) {
    destroy_model_struct(object->model);
    destroy_vertex3D_struct(object->location);
    free(object);
}