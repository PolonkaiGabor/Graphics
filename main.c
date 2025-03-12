#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <obj/load.h>
#include <obj/draw.h>

#define bool int
#define true 1
#define false 0

void drawCube() {
    glBegin(GL_QUADS); // Kocka 6 oldalának kirajzolása
    // Elülső oldal
    glColor3f(1.0f, 0.0f, 0.0f); // Piros
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, 1.0f);
    
    // Hátsó oldal
    glColor3f(0.0f, 1.0f, 0.0f); // Zöld
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Bal oldal
    glColor3f(0.0f, 0.0f, 1.0f); // Kék
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    // Jobb oldal
    glColor3f(1.0f, 1.0f, 0.0f); // Sárga
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);

    // Felső oldal
    glColor3f(1.0f, 0.0f, 1.0f); // Lila
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);

    // Alsó oldal
    glColor3f(0.0f, 1.0f, 1.0f); // Cián
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glEnd();
}

typedef struct {
    float x, y, z;          // Pozíció
    float rotationX, rotationY, rotationZ; // Forgatás
    float scaleX, scaleY, scaleZ; // Méretezés
} Object3D;

// Objektum létrehozása
Object3D createObject(float x, float y, float z, float rotationX, float rotationY, float rotationZ, float scaleX, float scaleY, float scaleZ) {
    Object3D obj;
    obj.x = x;
    obj.y = y;
    obj.z = z;
    obj.rotationX = rotationX;
    obj.rotationY = rotationY;
    obj.rotationZ = rotationZ;
    obj.scaleX = scaleX;
    obj.scaleY = scaleY;
    obj.scaleZ = scaleZ;
    return obj;
}

// Objektum rajzolása
void drawObject(Object3D* obj) {
    glPushMatrix();  
    glTranslatef(obj->x, obj->y, obj->z);  // Objektum elhelyezése
    glRotatef(obj->rotationX, 1.0f, 0.0f, 0.0f); // X tengely körüli forgatás
    glRotatef(obj->rotationY, 0.0f, 1.0f, 0.0f); // Y tengely körüli forgatás
    glRotatef(obj->rotationZ, 0.0f, 0.0f, 1.0f); // Z tengely körüli forgatás
    glScalef(obj->scaleX, obj->scaleY, obj->scaleZ); // Objektum méretezése

    // Kocka kirajzolása
    drawCube();

    glPopMatrix();
}

// Objektum módosítása
void updateObject(Object3D* obj, float deltaX, float deltaY, float deltaZ, float deltaRotationX, float deltaRotationY, float deltaRotationZ) {
    obj->x += deltaX;
    obj->y += deltaY;
    obj->z += deltaZ;
    obj->rotationX += deltaRotationX;
    obj->rotationY += deltaRotationY;
    obj->rotationZ += deltaRotationZ;
}


void initOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Háttérszín fekete
    glEnable(GL_DEPTH_TEST); // Aktiváljuk a mélységi tesztet
    glClearDepth(1.0f); // Maximális mélységi érték
    glDepthFunc(GL_LESS); // Az objektumok rendje
    glShadeModel(GL_SMOOTH); // Simított árnyékolás
    //glEnable(GL_LIGHTING); // Aktiváljuk a világítást
    glEnable(GL_LIGHT0); // Aktiváljuk az első fényt
    glEnable(GL_COLOR_MATERIAL); // Aktiváljuk a színt
    glEnable(GL_NORMALIZE);  // A normál vektorok normalizálása
}



int main(int argc, char* argv[]) {
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init hiba: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Forgó Kocka", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 800, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("SDL_CreateWindow hiba: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        printf("SDL_GL_CreateContext hiba: %s\n", SDL_GetError());
        return -1;
    }

    initOpenGL();

    // OpenGL projektálás beállítása (ortografikus)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)1000 / (float)800; // Képernyő arány
    glOrtho(-3.0f * aspect, 3.0f * aspect, -3.0f, 3.0f, -10.0f, 10.0f);
    glMatrixMode(GL_MODELVIEW);

 // Objektum létrehozása
 Object3D cube = createObject(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
// Object3D cube2 = createObject(0.0f, 3.0f, -2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

    SDL_Event e;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

      //  float random1 = (float)rand() / (float)RAND_MAX;
       //  float random2 = (float)rand() / (float)RAND_MAX;
      //  float random3 = (float)rand() / (float)RAND_MAX;

        // Objektum állapotának frissítése
        updateObject(&cube, 0.0f, 0.0f, 0.0f, 1.0f/100.0f, 1.0f/100.0f, 0.0f);
      //  updateObject(&cube2, 0.0f, 0.0f, 0.0f, 1.0f/100.0f, 0, 0.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Tisztítjuk a képernyőt


        // Kocka kirajzolása
        drawObject(&cube);
     //   drawObject(&cube2);

        SDL_GL_SwapWindow(window); // Frissítjük a képernyőt
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
