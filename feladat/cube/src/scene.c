#include "scene.h"
#include <stdio.h>

#include "load.c"
#include "draw.c"

const ObjectTemplate object_templates[] = {
    {0, "assets/models/cube.obj", "assets/textures/cube.png"},
    {1, "assets/models/cube.obj", "assets/textures/cube.png"},
    {10, "assets/models/cat.obj", "assets/textures/cube.png"}, // kuka
    // stb.
};
const int object_template_count = sizeof(object_templates) / sizeof(ObjectTemplate);


Model* objectCreateByID(Scene* scene, int id, float x, float y, float z) {
    for (int i = 0; i < object_template_count; i++) {
        if (object_templates[i].id == id) {
            if (scene->object_count >= 1024) {
                printf("Maximum objektumszám elérve.\n");
                return NULL;
            }

            // Betöltjük a modellt
            load_model(&scene->objects[scene->object_count], object_templates[i].model_path);

            // Beállítjuk a pozíciókat
            scene->objects[scene->object_count].pos_x = x;
            scene->objects[scene->object_count].pos_y = y;
            scene->objects[scene->object_count].pos_z = z;

            // Textúra betöltés
            scene->texture_ids[scene->object_count] = load_texture(object_templates[i].texture_path);

            printf("Sikeres object betoltes: ID=%d -> scene index %d\n", id, scene->object_count);

            // Visszaadjuk a pointert az új objektumra
            return &scene->objects[scene->object_count++];
        }
    }

    printf("Ismeretlen object ID: %d\n", id);
    return NULL;
}

bool setElementPosition(Model* element, float x, float y, float z) {
    if (element == NULL) {
        return false;
    }

    element->pos_x = x;
    element->pos_y = y;
    element->pos_z = z;

    return true;
}

#define GRID_STEP 0.1  // Térköz
#define GRID_SIZE 10   // Alapértelmezett oszlopok és sorok száma

// A háló kirajzolásáért felelős függvény
void draw_grid(int rows, int cols,int selected_row, int selected_col)
{
    glDisable(GL_LIGHTING);
     glDisable(GL_TEXTURE_2D);  // Textúra letiltása
    glColor3f(1.0, 1.0, 1.0);  // Fehér szín
    glLineWidth(2.0);  // Vonal vastagságának beállítása (pl. 2.0)

    glBegin(GL_LINES);

    // Sorok kirajzolása
    for (int i = -rows / 2; i <= rows / 2; i++) {
        glVertex3f(-GRID_STEP * cols / 2+GRID_STEP/2, GRID_STEP * i+GRID_STEP/2, 0.01);
        glVertex3f(GRID_STEP * cols / 2+GRID_STEP/2, GRID_STEP * i+GRID_STEP/2, 0.01);
    }

    // Oszlopok kirajzolása
    for (int i = -cols / 2; i <= cols / 2; i++) {
        glVertex3f(GRID_STEP * i+GRID_STEP/2, -GRID_STEP * rows / 2+GRID_STEP/2, 0.01);
        glVertex3f(GRID_STEP * i+GRID_STEP/2, GRID_STEP * rows / 2+GRID_STEP/2, 0.01);
    }

    glEnd();
        glColor3f(0.0, 0.0, 1.0);  // Kék szín
        glLineWidth(3.0);

    glBegin(GL_LINES);  // Négyzetet rajzolunk
        // Bal alsó pont -> Jobb alsó pont
        glVertex3f(GRID_STEP * selected_col - GRID_STEP / 2, GRID_STEP * selected_row - GRID_STEP / 2, 0.011);
        glVertex3f(GRID_STEP * selected_col + GRID_STEP / 2, GRID_STEP * selected_row - GRID_STEP / 2, 0.011);

        // Jobb alsó pont -> Jobb felső pont
        glVertex3f(GRID_STEP * selected_col + GRID_STEP / 2, GRID_STEP * selected_row - GRID_STEP / 2, 0.011);
        glVertex3f(GRID_STEP * selected_col + GRID_STEP / 2, GRID_STEP * selected_row + GRID_STEP / 2, 0.011);

        // Jobb felső pont -> Bal felső pont
        glVertex3f(GRID_STEP * selected_col + GRID_STEP / 2, GRID_STEP * selected_row + GRID_STEP / 2, 0.011);
        glVertex3f(GRID_STEP * selected_col - GRID_STEP / 2, GRID_STEP * selected_row + GRID_STEP / 2, 0.011);

        // Bal felső pont -> Bal alsó pont
        glVertex3f(GRID_STEP * selected_col - GRID_STEP / 2, GRID_STEP * selected_row + GRID_STEP / 2, 0.011);
        glVertex3f(GRID_STEP * selected_col - GRID_STEP / 2, GRID_STEP * selected_row - GRID_STEP / 2, 0.011);
    glEnd();

    glEnable(GL_TEXTURE_2D);  // Textúra engedélyezése (ha szükséges a többi objektumhoz)
    glEnable(GL_LIGHTING);
}


void init_scene(Scene* scene)
{
    // load_model(&(scene->cube), "assets/models/cube.obj");
    // scene->texture_id = load_texture("assets/textures/cube.png");
    scene->object_count = 0;
    printf("KEZDO SCENE = %d\n", scene->object_count);
    scene->plane_texture = load_texture("assets/textures/grass.jpg");
  /*  load_model(&(scene->objects[scene->object_count]), "assets/models/cube.obj");
    scene->texture_id = load_texture("assets/textures/cube.png");
    printf("Sikeres object betoltes, ID %d",scene->object_count);
    scene->object_count++;

    load_model(&(scene->objects[scene->object_count]), "assets/models/cat.obj");
    scene->texture_id = load_texture("assets/textures/cube.png");
    printf("Sikeres object betoltes, ID %d",scene->object_count);
    scene->object_count++; */

   // objectCreate(scene, "assets/models/cube.obj", "assets/textures/cube.png");
   // objectCreate(scene, "assets/models/cube.obj", "assets/textures/cube.png");

    //objectCreateByID(scene, MODEL_ID, X, Y, Z);
    Model* cube = objectCreateByID(scene,0,0.0,0.0,0.5);
    objectCreateByID(scene,1,0.0,2.0,0.5);
    objectCreateByID(scene,10,0.0,4.0,0.5);

    setElementPosition(cube, 0.0f, 0.0f, 3.0f); 

    scene->material.ambient.red = 1.0; // ha ezeket mind 1 re allitom akkor latszik minden is a texturabol mindig
    scene->material.ambient.green = 1.0;
    scene->material.ambient.blue = 1.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 1.0;
    scene->material.specular.green = 1.0;
    scene->material.specular.blue = 1.0;

    scene->material.shininess = 0.0;
}

void set_lighting()
{
    float ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   // float diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   // float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    //float position[] = { 1.0f, 1.0f, 1.0f, 1 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
   // glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene)
{
}

//GLuint ground_texture = load_texture("assets/textures/cube.png");

void draw_groundplane(const Scene* scene){
    
    glBindTexture(GL_TEXTURE_2D, scene->plane_texture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, -10.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, -10.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f,  10.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f,  10.0f, 0.0f);
    glEnd();

}

void draw_all_objects(const Scene* scene)
{
    for (int i = 0; i < scene->object_count; i++) {
        // Példa: eltolás objektumonként, hogy ne egymáson legyenek
     //   float x_offset = i * 2.0f;  // eltolás vízszintesen
     //   float y_offset = 0.0f;
     //   float z_offset = 0.5f;
        glBindTexture(GL_TEXTURE_2D, scene->texture_ids[i]);
        draw_model(&(scene->objects[i]), scene->objects[i].pos_x, scene->objects[i].pos_y, scene->objects[i].pos_z);
    }
}

void render_scene(const Scene* scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_origin();
        draw_groundplane(scene);
    draw_all_objects(scene);
    draw_grid(20, 20,-9,-9);  // 20 soros és 20 oszlopos háló, ezt módosíthatod

}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}
