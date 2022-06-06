#version 330 core

// "in" and "out" are used to pass data from the vertex shader to the fragment shader
in vec3 Color;
// Fragement shader must output a vec4 representing the color of the current vertex
out vec4 FragColor;

void main() {
    FragColor = vec4(Color, 1.0);
}
