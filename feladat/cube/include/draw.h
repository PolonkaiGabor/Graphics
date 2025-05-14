#ifndef OBJ_DRAW_H
#define OBJ_DRAW_H

#include "model.h"

/**
 * Draw the model.
 */
void draw_model(const Model* model,float x, float y, float z);

/**
 * Draw the triangles of the model.
 */
void draw_triangles(const Model* model,float pos_x, float pos_y, float pos_z);

#endif /* OBJ_DRAW_H */
