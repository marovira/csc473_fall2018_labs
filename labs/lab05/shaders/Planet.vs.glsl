#version 330 core

#include "LayoutLocations.glsl"
layout(location = VERTICES_LAYOUT_LOCATION) in vec3 position;
layout(location = NORMALS_LAYOUT_LOCATION) in vec3 normal;
layout(location = TEXTURES_LAYOUT_LOCATION) in vec2 tex;

out VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec3 eyeDirection;
    vec3 lightDirection;
    vec3 lightPosition;
} outData;

#include "UniformMatrices.glsl"

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

    outData.position = (model * vec4(position, 1.0)).xyz;

    vec3 vertexPos = (view * model * vec4(position, 1.0)).xyz;
    outData.eyeDirection = vec3(0, 0, 0) - vertexPos;

    outData.lightPosition = vec3(0, 10, 0);
    vec3 lightPos = (view * vec4(outData.lightPosition, 1.0)).xyz;
    outData.lightDirection = lightPos + outData.eyeDirection;

    outData.normal = (inverse(transpose(view * model)) * vec4(normal, 0)).xyz;
    outData.texCoord = tex;
}

