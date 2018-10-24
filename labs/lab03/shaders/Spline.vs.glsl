#version 330 core

#include "LayoutLocations.glsl"
layout(location = VERTICES_LAYOUT_LOCATION) in vec3 position;

#include "UniformMatrices.glsl"

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
}
