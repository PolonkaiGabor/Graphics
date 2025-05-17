#include "scene.h"
#include <stdio.h>
#include <string.h>

#include "load.c"
#include "draw.c"
#include "info.h"

#include <SDL2/SDL.h>

ObjectTemplate object_templates[] = {
    { .id = 0, .model_path = "assets/models/cube.obj", .texture_path = "assets/textures/cube.png" },
    { .id = 1, .model_path = "assets/models/cube.obj", .texture_path = "assets/textures/cube.png" },
    { .id = 2, .model_path = "assets/models/floor5.obj", .texture_path = "assets/textures/wood_floor.jpg" },
    { .id = 10, .model_path = "assets/models/cat.obj", .texture_path = "assets/textures/cube.png" }, 
    // stb.
};
const int object_template_count = sizeof(object_templates) / sizeof(ObjectTemplate);

TextureEntry textures[] = {
    { 0, 0, "assets/textures/wood_floor.png", false },
    { 1, 0, "assets/textures/black_stones_floor.jpg", false },
    { 2, 0, "assets/textures/rustic_wood_floor.jpg", false }
};

const int textureEntry_count = sizeof(textures) / sizeof(TextureEntry);

void init_templates() {
    for (int i = 0; i < object_template_count; i++) {
        object_templates[i].model_loaded = false;
        object_templates[i].texture_loaded = false;
        memset(&object_templates[i].cached_model, 0, sizeof(Model));
        object_templates[i].cached_texture_id = 0;
    }
}


Model* objectCreateByID(Scene* scene, int id, float x, float y, float z) {
    for (int i = 0; i < object_template_count; i++) {
        if (object_templates[i].id == id) {
            if (scene->object_count >= 1024) {
                printf("Maximum objektumszám elérve.\n");
                return NULL;
            }

            // Modell cache betöltése
            if (!object_templates[i].model_loaded) { //igy gyorsabb a betoltes mivel nem kell minden modelt egyesevvel ujra olvastatni
                load_model(&object_templates[i].cached_model, object_templates[i].model_path);
                object_templates[i].model_loaded = true;
            }

            // Textúra cache betöltése
            if (!object_templates[i].texture_loaded) {
                object_templates[i].cached_texture_id = load_texture(object_templates[i].texture_path);
                object_templates[i].texture_loaded = true;
            }

            // Objektum példányosítás
            scene->objects[scene->object_count] = object_templates[i].cached_model;

            // Beállítjuk a pozíciókat
            scene->objects[scene->object_count].pos_x = x;
            scene->objects[scene->object_count].pos_y = y;
            scene->objects[scene->object_count].pos_z = z;

            // Textúra betöltés
            scene->texture_ids[scene->object_count] = object_templates[i].cached_texture_id;

            printf("Sikeres object betoltes: ID=%d -> scene index %d\n", id, scene->object_count);

            // Visszaadjuk a pointert az új objektumra
            return &scene->objects[scene->object_count++];
        }
    }

    printf("Ismeretlen object ID: %d\n", id);
    return NULL;
}

int getSceneObjectIndexByModel(Scene* scene, Model* model) {
    for (int i = 0; i < scene->object_count; i++) {
        if (&scene->objects[i] == model) {
            return i;  // Megtaláltuk: visszaadjuk az indexet
        }
    }
    return -1; // Nem található
}

int get_texture_index(int key) {
    for (int i = 0; i < textureEntry_count; i++) {
        if (textures[i].texture_key == key) {
            return i;
        }
    }
    return -1; // nem található
}

void setElementTexture(Scene* scene,Model* element,int textureKeyID){
    int textureIndex = get_texture_index(textureKeyID);
    if(textureIndex == -1){
        printf("nemtalalhato ilyen textura\n");
        return;
    }
    if (!textures[textureIndex].texture_loaded) {
        textures[textureIndex].texture_id = load_texture((char*)textures[textureIndex].texture_path);
        textures[textureIndex].texture_loaded = true;
    }

    int index = getSceneObjectIndexByModel(scene,element);
    scene->texture_ids[index] = textures[textureIndex].texture_id;
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

#define GRID_STEP 1  // Térköz
#define GRID_SIZE 10   // Alapértelmezett oszlopok és sorok száma

// A háló kirajzolásáért felelős függvény
void draw_grid(const Scene* scene)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);  // Textúra letiltása


    //Élsimítás 
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0, 1.0, 1.0, 1.0f);// Fehér szín átlátszó
    glLineWidth(3.0);  // Vonal vastagságának beállítása (pl. 2.0)


    glBegin(GL_LINES);

    // Sorok kirajzolása
    for (int i = 0; i <= scene->grid.max_row; i++) {
        float y = i * GRID_STEP + GRID_STEP / 2.0;
        float x_start = GRID_STEP / 2.0;
        float x_end = GRID_STEP * scene->grid.max_col + GRID_STEP / 2.0;

        glVertex3f(x_start, y, 0.02f);
        glVertex3f(x_end, y, 0.02f);
    }

    // Oszlopok kirajzolása
    for (int i = 0; i <= scene->grid.max_col; i++) {
        float x = i * GRID_STEP + GRID_STEP / 2.0;
        float y_start = GRID_STEP / 2.0;
        float y_end = scene->grid.max_col * GRID_STEP + GRID_STEP / 2.0;

        glVertex3f(x, y_start, 0.02f);
        glVertex3f(x, y_end, 0.02f);
    }

    glEnd();

    glColor3f(0.0, 0.0, 1.0);  // Kék szín
    glLineWidth(3.0);

    if (scene->grid.selection_start[0] != -1) {

        int row_start = scene->grid.selection_start[0];
        int col_start = scene->grid.selection_start[1];
        int row_end = row_start + scene->grid.selected_row_count;
        int col_end = col_start + scene->grid.selected_col_count;

        // Kiszámoljuk a min és max értékeket, hogy helyesen rajzoljon negatív érték esetén is
        if (row_end < row_start) {
            int tmp = row_start; row_start = row_end; row_end = tmp;
        }
        if (col_end < col_start) {
            int tmp = col_start; col_start = col_end; col_end = tmp;
        }

        for (int r = row_start; r <= row_end; r++) {
            for (int c = col_start; c <= col_end; c++) {
                float x = c * GRID_STEP + GRID_STEP / 2.0;
                float y = r * GRID_STEP + GRID_STEP / 2.0;

                glBegin(GL_LINES);
                    glVertex3f(x, y, 0.03f);
                    glVertex3f(x + GRID_STEP, y, 0.03f);

                    glVertex3f(x + GRID_STEP, y, 0.03f);
                    glVertex3f(x + GRID_STEP, y + GRID_STEP, 0.03f);

                    glVertex3f(x + GRID_STEP, y + GRID_STEP, 0.03f);
                    glVertex3f(x, y + GRID_STEP, 0.03f);

                    glVertex3f(x, y + GRID_STEP, 0.03f);
                    glVertex3f(x, y, 0.03f);
                glEnd();
            }
        }
    } else {
        // Csak egy cellát rajzolunk ki a selected_row és selected_col alapján
        float x = (scene->grid.selected_col) * GRID_STEP + GRID_STEP / 2.0;
        float y = (scene->grid.selected_row) * GRID_STEP + GRID_STEP / 2.0;

        glBegin(GL_LINES);
            glVertex3f(x, y, 0.03f);
            glVertex3f(x + GRID_STEP, y, 0.03f);

            glVertex3f(x + GRID_STEP, y, 0.03f);
            glVertex3f(x + GRID_STEP, y + GRID_STEP, 0.03f);

            glVertex3f(x + GRID_STEP, y + GRID_STEP, 0.03f);
            glVertex3f(x, y + GRID_STEP, 0.03f);

            glVertex3f(x, y + GRID_STEP, 0.03f);
            glVertex3f(x, y, 0.03f);
        glEnd();
    }

    glEnable(GL_TEXTURE_2D);  // Textúra engedélyezése (ha szükséges a többi objektumhoz)
    glEnable(GL_LIGHTING);

    //Élsimítás kikapcsolása
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
}

void createFloorObject(Scene* scene, int row, int col, int textureID){
    float x = (col * GRID_STEP)+(GRID_STEP);
    float y = (row * GRID_STEP)+(GRID_STEP);
    float z = 0.0f;

    Model* floor = objectCreateByID(scene, 2, x, y, z);
    if(textureID != 0){
        setElementTexture(scene,floor,textureID);
    }
    scene->grid.cells[row][col].object = floor;
    scene->grid.cells[row][col].texture_id = textureID;
}

void init_grid(Grid* grid, int rows, int cols)
{
    grid->max_row = rows;
    grid->max_col = cols;

    grid->selected_row = 0;
    grid->selected_col = 0;

    grid->selected_col_count = 0;
    grid->selected_row_count = 0;

    grid->selection_start[0] = -1;
    grid->selection_start[1] = -1;


    grid->cells = (Cell**)malloc(rows * sizeof(Cell*));
    if (grid->cells == NULL) {
        fprintf(stderr, "Memory allocation failed for rows.\n");
        exit(1);
    }

    for (int r = 0; r < rows; r++) {
        grid->cells[r] = (Cell*)malloc(cols * sizeof(Cell));
        if (grid->cells[r] == NULL) {
            fprintf(stderr, "Memory allocation failed for columns in row %d.\n", r);
            exit(1);
        }

        for (int c = 0; c < cols; c++) {
            grid->cells[r][c].occupied = 0;
           // grid->cells[r][c].texture_id = 0;
            // opcionálisan: memset a modelre, vagy egyéb inicializálás
        }
    }

}

void init_scene(Scene* scene)
{
    scene->object_count = 0;
    init_templates();
    init_grid(&scene->grid, 20, 20);


    printf("KEZDO SCENE = %d\n", scene->object_count);
    scene->plane_texture = load_texture("assets/textures/grass.jpg");

    Model* cube = objectCreateByID(scene,0,0.0,0.0,0.5);
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
  //  draw_bounding_box(&scene->objects[2]);
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
