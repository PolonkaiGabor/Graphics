#include "app.h"
#include <stdio.h>
#include <GL/glu.h>

#include <SDL2/SDL_image.h>

#include <SDL2/SDL.h>
#include "SDL2/SDL_mixer.h"

void init_app(App* app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "The Sims 5",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    /////////////////////////////////////////////
    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }
  
    //app icon beallitas
    SDL_Surface* icon = IMG_Load("assets/textures/icon.png");  // vagy: SDL_LoadBMP("icon.bmp")
    if (icon == NULL) {
        printf("Nem sikerült betölteni az ikont: %s\n", IMG_GetError());
    } else {
        SDL_SetWindowIcon(app->window, icon);  // ikon beállítása
        SDL_FreeSurface(icon);            // felszabadítás beállítás után
    }
    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));
   // init_character(&app->player);

    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-.08, .08, -.06, .06, 0.1, 200);
    
}

typedef struct {
    double x, y, z;
} Point3D;

#define MAX_POINTS 1000
Point3D points[MAX_POINTS];
int point_count = 0;

void handle_app_events(App* app)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;
   // const Uint8* keystates = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), 5);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -5);
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), 5);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -5);
                break;
            case SDL_SCANCODE_LCTRL:
                set_camera_z_speed(&(app->camera), -5.0);
                break;
            case SDL_SCANCODE_SPACE:
                set_camera_z_speed(&(app->camera), 5.0);
                break;
            case SDL_SCANCODE_TAB:

                break;

            case SDL_SCANCODE_DOWN:
                    if (app->scene.grid.selected_row > 0)
                    {
                        app->scene.grid.selected_row--;

                        if (app->scene.grid.selection_start[0] != -1 && app->scene.grid.selection_start[1] != -1) { //keystates[SDL_SCANCODE_LSHIFT]    
                            app->scene.grid.selected_row_count--;
                           // printf("SHIFT + DOWN -> Lefele mozgas shift-tel ROW: %d\n",app->scene.grid.selected_row_count);
                        } else {
                           // printf("DOWN -> Lefele mozgas\n");
                        }
                    }
                    break;
            case SDL_SCANCODE_UP:
                    if (app->scene.grid.selected_row < app->scene.grid.max_row - 1)
                    {
                        app->scene.grid.selected_row++;

                        if (app->scene.grid.selection_start[0] != -1 && app->scene.grid.selection_start[1] != -1) { //keystates[SDL_SCANCODE_LSHIFT]
                            app->scene.grid.selected_row_count++;
                           // printf("SHIFT + UP -> Felfele mozgas shift-tel ROW: %d\n",app->scene.grid.selected_row_count);
                        } else {
                           // printf("UP -> Felfele mozgas\n");
                        }
                    }
                    break;
            case SDL_SCANCODE_LEFT:
                    if (app->scene.grid.selected_col > 0){
                        app->scene.grid.selected_col--;

                        if (app->scene.grid.selection_start[0] != -1 && app->scene.grid.selection_start[1] != -1) { //keystates[SDL_SCANCODE_LSHIFT]    
                            app->scene.grid.selected_col_count--;
                            //printf("SHIFT + LEFT -> Balra mozgas shift-tel ROW: %d\n",app->scene.grid.selected_col_count);
                        } else {
                           // printf("LEFT -> Balra mozgas\n");
                        }
                    }
                    break;
            case SDL_SCANCODE_RIGHT:
                    if (app->scene.grid.selected_col < app->scene.grid.max_col - 1){ //mivel maxrow 20 de 0 tol indexelve 19 kell legyen a maxnak
                        app->scene.grid.selected_col++;

                        if (app->scene.grid.selection_start[0] != -1 && app->scene.grid.selection_start[1] != -1) { //keystates[SDL_SCANCODE_LSHIFT]    
                            app->scene.grid.selected_col_count++;
                            //printf("SHIFT + RIGHT -> Jobbra mozgas shift-tel ROW: %d\n",app->scene.grid.selected_col_count);
                        } else {
                           // printf("RIGHT -> Jobbra mozgas\n");
                        }
                    }
                    break;
            case SDL_SCANCODE_RETURN:
                    if(app->scene.grid.selection_start[0] != -1 && app->scene.grid.selection_start[1] != -1){
                        // Ha -1, akkor az egész kijelölést rajzoljuk
                        int row_start = app->scene.grid.selection_start[0];
                        int col_start = app->scene.grid.selection_start[1];
                        int row_end = row_start + app->scene.grid.selected_row_count;
                        int col_end = col_start + app->scene.grid.selected_col_count;

                        // Kiszámoljuk a min és max értékeket, hogy helyesen rajzoljon negatív érték esetén is
                        if (row_end < row_start) {
                            int tmp = row_start; row_start = row_end; row_end = tmp;
                        }
                        if (col_end < col_start) {
                            int tmp = col_start; col_start = col_end; col_end = tmp;
                        }
                        for (int r = row_start; r <= row_end; r++) {
                            for (int c = col_start; c <= col_end; c++) {
                                if(app->scene.grid.cells[r][c] != 1){
                                    app->scene.grid.cells[r][c] = 1;
                                    createFloorObject(&app->scene, r, c);
                                }
                            }
                        }
                        

                        app->scene.grid.selected_col_count = 0;
                        app->scene.grid.selected_row_count = 0;

                        app->scene.grid.selection_start[0] = -1;
                        app->scene.grid.selection_start[1] = -1;
                    } else{
                        app->scene.grid.cells[app->scene.grid.selected_row][app->scene.grid.selected_col] = 1;
                    }
                    
                    break;
            case SDL_SCANCODE_LSHIFT:
                    if(app->scene.grid.selection_start[0] == -1 || app->scene.grid.selection_start[1] == -1){
                        app->scene.grid.selection_start[0] = app->scene.grid.selected_row;
                        app->scene.grid.selection_start[1] = app->scene.grid.selected_col;
                      //  printf("SELECTION START POS: ROW: %d, COL: %d\n",app->scene.grid.selection_start[0],app->scene.grid.selection_start[1]);
                    }
                    break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_LCTRL:
            case SDL_SCANCODE_SPACE:
                set_camera_z_speed(&(app->camera), 0.0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
        /*    if (event.button.button == SDL_BUTTON_LEFT && point_count < MAX_POINTS) {
                int mouse_x = event.button.x;
                int mouse_y = event.button.y;

                GLint viewport[4];
                GLdouble modelview[16];
                GLdouble projection[16];
                GLfloat winX, winY, winZ;
                GLdouble worldX, worldY, worldZ;

                glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
                glGetDoublev(GL_PROJECTION_MATRIX, projection);
                glGetIntegerv(GL_VIEWPORT, viewport);

                winX = (float)mouse_x;
                winY = (float)(viewport[3] - mouse_y);  // SDL és OpenGL Y tengely különbség

                glReadPixels(mouse_x, viewport[3] - mouse_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

                if (gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ)) {
                    //points[point_count].x = worldX;
                    //points[point_count].y = worldY;
                    //points[point_count].z = worldZ;
                    //point_count++;
                    objectCreateByID(&(app->scene), 10, worldX, worldY, worldZ);
                    printf("New point at: %f %f %f\n", worldX, worldY, worldZ);
                }
            } */
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down) {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}


#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS)

void update_app(App* app)
{
    double current_time;
    double elapsed_time;
    Uint32 frame_start, frame_time;

    current_time = (double)SDL_GetTicks() / 1000;
    frame_start = SDL_GetTicks();  // Időpont kezdés
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    
    update_camera(&(app->camera), elapsed_time);
 //   update_character(&app->player, elapsed_time, app);

    update_scene(&(app->scene));

    frame_time = SDL_GetTicks() - frame_start;

  //  float fps = 1000.0f / (float)(frame_time > 0 ? frame_time : 1);
   // printf("FPS: %.2f\n", fps);

    if (frame_time < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frame_time);  // Várakozz, hogy elérjük az FPS célt
        }

}

void render_app(App* app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app)
{
    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
