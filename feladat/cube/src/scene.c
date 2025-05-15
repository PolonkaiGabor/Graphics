#include "scene.h"
#include <stdio.h>
#include <string.h>

#include "load.c"
#include "draw.c"
#include "info.h"

#include <SDL2/SDL.h>

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
void draw_grid(const Scene* scene)
{
    glDisable(GL_LIGHTING);
     glDisable(GL_TEXTURE_2D);  // Textúra letiltása
    glColor4f(1.0, 1.0, 1.0, 0.75f);// Fehér szín átlátszó
    glLineWidth(2.0);  // Vonal vastagságának beállítása (pl. 2.0)

    //Élsimítás 
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glBegin(GL_LINES);

    // Sorok kirajzolása
    for (int i = 0; i <= scene->grid.max_row; i++) {
        //float y = (i - scene->grid.max_row / 2) * GRID_STEP + GRID_STEP / 2;
        //float x_start = -GRID_STEP * scene->grid.max_col / 2 + GRID_STEP / 2;
        //float x_end = GRID_STEP * scene->grid.max_col / 2 + GRID_STEP / 2;

        float y = i * GRID_STEP + GRID_STEP / 2;
        float x_start = GRID_STEP / 2;
        float x_end = GRID_STEP * scene->grid.max_col + GRID_STEP / 2;

        glVertex3f(x_start, y, 0.01f);
        glVertex3f(x_end, y, 0.01f);
    }

    // Oszlopok kirajzolása
    for (int i = 0; i <= scene->grid.max_col; i++) {
        //float x = (i - scene->grid.max_col / 2) * GRID_STEP + GRID_STEP / 2;
        //float y_start = -GRID_STEP * scene->grid.max_row / 2 + GRID_STEP / 2;
       // float y_end = GRID_STEP * scene->grid.max_row / 2 + GRID_STEP / 2;

        float x = i * GRID_STEP + GRID_STEP / 2;
        float y_start = GRID_STEP / 2;
        float y_end = scene->grid.max_col * GRID_STEP + GRID_STEP / 2;

        glVertex3f(x, y_start, 0.01f);
        glVertex3f(x, y_end, 0.01f);
    }

    glEnd();

    glColor3f(0.0, 0.0, 1.0);  // Kék szín
    glLineWidth(3.0);

   /* float x = (scene->grid.selected_col - scene->grid.max_col / 2) * GRID_STEP;
    float y = (scene->grid.selected_row - scene->grid.max_row / 2) * GRID_STEP;
    glBegin(GL_LINES);  // Négyzetet rajzolunk
        // Bal alsó pont -> Jobb alsó pont
        glVertex3f(x - GRID_STEP / 2, y - GRID_STEP / 2, 0.011);
        glVertex3f(x + GRID_STEP / 2, y - GRID_STEP / 2, 0.011);

        // Jobb alsó pont -> Jobb felső pont
        glVertex3f(x + GRID_STEP / 2, y - GRID_STEP / 2, 0.011);
        glVertex3f(x + GRID_STEP / 2, y + GRID_STEP / 2, 0.011);

        // Jobb felső pont -> Bal felső pont
        glVertex3f(x + GRID_STEP / 2, y + GRID_STEP / 2, 0.011);
        glVertex3f(x - GRID_STEP / 2, y + GRID_STEP / 2, 0.011);

        // Bal felső pont -> Bal alsó pont
        glVertex3f(x - GRID_STEP / 2, y + GRID_STEP / 2, 0.011);
        glVertex3f(x - GRID_STEP / 2, y - GRID_STEP / 2, 0.011);
    glEnd(); */

    float x = (scene->grid.selected_col) * GRID_STEP + GRID_STEP / 2; // Bal alsó sarok x koordinátája
    float y = (scene->grid.selected_row) * GRID_STEP + GRID_STEP / 2; // Bal alsó sarok y koordinátája

    glBegin(GL_LINES);  // Négyzetet rajzolunk
        // Bal alsó pont -> Jobb alsó pont
        glVertex3f(x, y, 0.011f);
        glVertex3f(x + GRID_STEP, y, 0.011f);

        // Jobb alsó pont -> Jobb felső pont
        glVertex3f(x + GRID_STEP, y, 0.011f);
        glVertex3f(x + GRID_STEP, y + GRID_STEP, 0.011f);

        // Jobb felső pont -> Bal felső pont
        glVertex3f(x + GRID_STEP, y + GRID_STEP, 0.011f);
        glVertex3f(x, y + GRID_STEP, 0.011f);

        // Bal felső pont -> Bal alsó pont
        glVertex3f(x, y + GRID_STEP, 0.011f);
        glVertex3f(x, y, 0.011f);
    glEnd();

    for (int i = 0; i < scene->grid.max_row; i++)
    {
        for (int j = 0; j < scene->grid.max_col; j++)
        {
            if (scene->grid.cells[i][j] == 1)
            {
                // Számoljuk ki a pozíciót úgy, hogy (x,y) legyen a bal alsó sarok, nem pedig középpont
                // Először számoljuk ki az eltolást, hogy a grid középen legyen, majd innen lépkedünk
               // float offsetX = - (scene->grid.max_col * GRID_STEP) / 2.0f;
              //  float offsetY = - (scene->grid.max_row * GRID_STEP) / 2.0f;

                float x = j * GRID_STEP + GRID_STEP/2;
                float y = i * GRID_STEP + GRID_STEP/2;

                glColor3f(0.5f, 0.5f, 0.5f); // szurke szin
                glBegin(GL_QUADS);
                    glVertex3f(x, y, 0.02f);                          // bal alsó
                    glVertex3f(x + GRID_STEP, y, 0.02f);             // jobb alsó
                    glVertex3f(x + GRID_STEP, y + GRID_STEP, 0.02f); // jobb felső
                    glVertex3f(x, y + GRID_STEP, 0.02f);             // bal felső
                glEnd();
            }
        }
    }

    glEnable(GL_TEXTURE_2D);  // Textúra engedélyezése (ha szükséges a többi objektumhoz)
    glEnable(GL_LIGHTING);

    //Élsimítás kikapcsolása
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}

void init_grid(Grid* grid, int rows, int cols)
{
    grid->max_row = rows;
    grid->max_col = cols;
    grid->selected_row = 0;
    grid->selected_col = 0;

    // Lefoglaljuk a sorokhoz a pointer tömböt
    grid->cells = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        grid->cells[i] = (int*)malloc(cols * sizeof(int));
        // Opcionális: nullázás
        memset(grid->cells[i], 0, cols * sizeof(int));
    }
}

void free_grid(Grid* grid)
{
    for (int i = 0; i < grid->max_row; i++) {
        free(grid->cells[i]);
    }
    free(grid->cells);
    grid->cells = NULL;
}

void init_scene(Scene* scene)
{
    scene->object_count = 0;
    init_grid(&scene->grid, 20, 20);


    printf("KEZDO SCENE = %d\n", scene->object_count);
    scene->plane_texture = load_texture("assets/textures/grass.jpg");

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
    (void)scene;
}

//GLuint ground_texture = load_texture("assets/textures/cube.png");

void draw_groundplane(const Scene* scene){
    
    glBindTexture(GL_TEXTURE_2D, scene->plane_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
    draw_bounding_box(&scene->objects[2]);
    draw_grid(scene);

}

void draw_origin()
{
    glDisable(GL_LIGHTING);
     glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0.02);
    glVertex3f(1, 0, 0.02);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0.02);
    glVertex3f(0, 1, 0.02);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0.02);
    glVertex3f(0, 0, 1.02);

    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
}
