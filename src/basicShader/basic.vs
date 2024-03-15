#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 vertexColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

int main() {
        gl_Position = projection * view * vec4(FragPos, 1.0);
        vertexColor = vec4(aPos, 1.0);
}
