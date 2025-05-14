#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include "model.h"

typedef struct Scene
{
    //Model cube;
    Model objects[1024];
    int object_count;
    Material material;
    GLuint texture_ids[1024];
    GLuint plane_texture;
} Scene;

typedef struct ObjectTemplate{
    int id;
    char* model_path;
    char* texture_path;
} ObjectTemplate;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting();

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

bool setElementPosition(Model* element, float x, float y, float z);

Model* objectCreateByID(Scene* scene, int id, float x, float y, float z);

#endif /* SCENE_H */
