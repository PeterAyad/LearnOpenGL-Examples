#version 330 core

// Vertex shader is supposed to do transformations on vertices

// Only vertex shader can read from VBO as it is the first shader in the pipeline
// This tells OpenGL to get the data (attributes) in VBO at index (location) of 0
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

// "in" and "out" are used to pass data from the vertex shader to the fragment shader
out vec3 Color;

void main() {
    Color = color;
    // gl_position is the output of vertex shader (position of the vertex after processing)
    gl_Position = vec4(position, 1.0);

    // There is also gl_VertexID which is the index of the current vertex
}
