#include "info.h"

#include <stdio.h>
#include <GL/gl.h> 

void print_model_info(const Model* model)
{
    printf("Vertices: %d\n", model->n_vertices);
    printf("Texture vertices: %d\n", model->n_texture_vertices);
    printf("Normals: %d\n", model->n_normals);
    printf("Triangles: %d\n", model->n_triangles);
}

void print_bounding_box(const Model* model)
{
    int i;
    double x, y, z;
    double min_x, max_x, min_y, max_y, min_z, max_z;

    if (model->n_vertices == 0) {
        return;
    }

    min_x = model->vertices[1].x + model->pos_x;
    max_x = model->vertices[1].x + model->pos_x;

    min_y = model->vertices[1].y + model->pos_y;
    max_y = model->vertices[1].y + model->pos_y;
   // printf("KAPJA %lf\n",model->vertices[0].y);

    min_z = model->vertices[1].z + model->pos_z;
    max_z = model->vertices[1].z + model->pos_z;
    printf("model->n_vertices = %d\n", model->n_vertices);
    for (i = 1; i <= model->n_vertices; ++i) {
        printf("vertex %d: x=%lf y=%lf z=%lf\n", i,
           model->vertices[i].x,
           model->vertices[i].y,
           model->vertices[i].z);
        x = model->vertices[i].x + model->pos_x;
        y = model->vertices[i].y + model->pos_y;
        z = model->vertices[i].z + model->pos_z;
        if (x < min_x) {
            min_x = x;
        }
        else if (x > max_x) {
            max_x = x;
        }
        if (y < min_y) {
            min_y = y;
        }
        else if (y > max_y) {
            max_y = y;
            
        }
        if (z < min_z) {
            min_z = z;
        }
        else if (z > max_z) {
            max_z = z;
        }
    }

    printf("Bounding box:\n");
    printf("x in [%lf, %lf]\n", min_x, max_x);
    printf("y in [%lf, %lf]\n", min_y, max_y);
    printf("z in [%lf, %lf]\n", min_z, max_z);
}

void draw_bounding_box(const Model* model) {
    if (model->n_vertices == 0) {
        return;
    }

    double min_x, max_x, min_y, max_y, min_z, max_z;
    double x, y, z;
    int i;

    // Kezdeti értékek az első vertex alapján (index 0-tól, mert C-ben így szokás)
    min_x = max_x = model->vertices[1].x + model->pos_x;
    min_y = max_y = model->vertices[1].y + model->pos_y;
    min_z = max_z = model->vertices[1].z + model->pos_z;

    for (i = 1; i <= model->n_vertices; ++i) {
        x = model->vertices[i].x + model->pos_x;
        y = model->vertices[i].y + model->pos_y;
        z = model->vertices[i].z + model->pos_z;

        if (x < min_x) min_x = x;
        if (x > max_x) max_x = x;
        if (y < min_y) min_y = y;
        if (y > max_y) max_y = y;
        if (z < min_z) min_z = z;
        if (z > max_z) max_z = z;
    }

    // Bounding box 8 sarka
    GLfloat vertices[8][3] = {
        { (float)min_x, (float)min_y, (float)min_z },
        { (float)max_x, (float)min_y, (float)min_z },
        { (float)max_x, (float)max_y, (float)min_z },
        { (float)min_x, (float)max_y, (float)min_z },
        { (float)min_x, (float)min_y, (float)max_z },
        { (float)max_x, (float)min_y, (float)max_z },
        { (float)max_x, (float)max_y, (float)max_z },
        { (float)min_x, (float)max_y, (float)max_z }
    };

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    // Zöld szín és vonalvastagság beállítása
    glColor3f(0.0f, 1.0f, 0.0f);  // zöld
    glLineWidth(3.0f);

    // Vonalrajz: a doboz élei (12 darab él)
    glBegin(GL_LINES);

    // Alsó lap (4 éle)
    glVertex3fv(vertices[0]); glVertex3fv(vertices[1]);
    glVertex3fv(vertices[1]); glVertex3fv(vertices[2]);
    glVertex3fv(vertices[2]); glVertex3fv(vertices[3]);
    glVertex3fv(vertices[3]); glVertex3fv(vertices[0]);

    // Felső lap (4 éle)
    glVertex3fv(vertices[4]); glVertex3fv(vertices[5]);
    glVertex3fv(vertices[5]); glVertex3fv(vertices[6]);
    glVertex3fv(vertices[6]); glVertex3fv(vertices[7]);
    glVertex3fv(vertices[7]); glVertex3fv(vertices[4]);

    // Függőleges élek (4 darab)
    glVertex3fv(vertices[0]); glVertex3fv(vertices[4]);
    glVertex3fv(vertices[1]); glVertex3fv(vertices[5]);
    glVertex3fv(vertices[2]); glVertex3fv(vertices[6]);
    glVertex3fv(vertices[3]); glVertex3fv(vertices[7]);

    glEnd();

    // Vonalszélesség visszaállítása alapra, ha szeretnéd
    glLineWidth(1.0f);
    glEnable(GL_TEXTURE_2D);  // Textúra engedélyezése (ha szükséges a többi objektumhoz)
    glEnable(GL_LIGHTING);
}