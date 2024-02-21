targets: cameras.o shaders.o dynarray.o
	gcc src/graphics.c include/glad.c apps/camera.o apps/shader.o apps/dynarray.o -o apps/app -ldl -lglfw -lm -g
	./apps/app
cameras.o:
	gcc -c include/cameras/camera.c -lm -g -o apps/camera.o 

shaders.o:
	gcc -c include/shaders/shader.c -g -o apps/shader.o
dynarray.o:
	gcc -c include/dynarray/dynarray.c -g -o apps/dynarray.o
