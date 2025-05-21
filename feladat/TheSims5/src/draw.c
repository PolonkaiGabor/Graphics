#include "draw.h"

#include <GL/gl.h>

void draw_model(const Model* model)
{
    float pos_x=model->pos_x, pos_y=model->pos_y, pos_z=model->pos_z;

    glPushMatrix();  // Mentjük a jelenlegi transzformációs mátrixot

    // Elmozdítás a megfelelő helyre
    glTranslatef(pos_x, pos_y, pos_z);

    // Forgatások Z-Y-X sorrendben (Gimbal lock elkerülésére ez a leggyakoribb)
    glRotatef(model->rot_z, 0.0f, 0.0f, 1.0f);  // Z tengely körül
    glRotatef(model->rot_y, 0.0f, 1.0f, 0.0f);  // Y tengely körül
    glRotatef(model->rot_x, 1.0f, 0.0f, 0.0f);  // X tengely körül

    // Model kirajzolása 0,0,0 helyre, mert már elmozgattuk a világot a model helyére
    draw_triangles(model);

    glPopMatrix();  // Visszaállítjuk a korábbi transzformációs mátrixot
}

void draw_triangles(const Model* model)
{
    int i, k;
    int vertex_index, texture_index, normal_index;
    float x, y, z, u, v;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < model->n_triangles; ++i) {
        for (k = 0; k < 3; ++k) {

            normal_index = model->triangles[i].points[k].normal_index;
            x = model->normals[normal_index].x;
            y = model->normals[normal_index].y;
            z = model->normals[normal_index].z;
            glNormal3f(x, y, z);

            texture_index = model->triangles[i].points[k].texture_index;
            u = model->texture_vertices[texture_index].u;
            v = model->texture_vertices[texture_index].v;
            glTexCoord2f(u, 1.0 - v);

            vertex_index = model->triangles[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3f(x, y, z);
        }
    }

    glEnd();
}
