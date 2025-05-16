all:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/scene.c src/info.c src/texture.c src/utils.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lobj -lglu32 -lopengl32 -lm resource.res -o "The Sims 5.exe" -Wall -Wextra -Wpedantic

linux:
	gcc -Iinclude/ src/app.c src/camera.c src/main.c src/scene.c src/texture.c src/utils.c -lobj -lSDL2 -lSDL2_image -lGL -lm -o cube -Wall -Wextra -Wpedantic

