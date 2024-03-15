# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lm -ldl -lglfw -g -lassimp

all: cameras.o shaders.o physics.o dynTexture.o dynVertex.o dynIndex.o dynMesh.o mesh.o model.o physics.o
	gcc src/graphics.c include/glad.c apps/physics.o apps/shader.o apps/camera.o apps/dynTexture.o apps/dynVertex.o apps/dynIndex.o apps/dynMesh.o apps/mesh.o apps/model.o -o apps/test $(LDFLAGS) $(CFLAGS)
	./apps/test
	
	#apps/dynTexture.o apps/dynVertex.o apps/dynIndex.o apps/dynMesh.o apps/mesh.o apps/model.o 
cameras.o:
	gcc -c include/cameras/camera.c -lm -g -o apps/camera.o
shaders.o:
	gcc -c include/shaders/shader.c -g -o apps/shader.o
dynTexture.o:
	gcc -c include/assimp/TextureType/dynTexture.c -o apps/dynTexture.o
dynVertex.o:
	gcc -c include/assimp/VertexType/dynVertex.c -o apps/dynVertex.o
dynIndex.o:
	gcc -c include/assimp/IndexType/dynIndex.c -o apps/dynIndex.o
dynMesh.o:
	gcc -c include/assimp/MeshType/dynMesh.c -o apps/dynMesh.o
mesh.o:
	gcc -c include/assimp/mesh/mesh.c -o apps/mesh.o
model.o:
	gcc -c include/assimp/model/model.c -o apps/model.o
physics.o:
	gcc -c include/physics/physicsObject.c -o apps/physics.o
