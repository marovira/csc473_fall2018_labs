#version 330 core

uniform vec3 colour;

out vec4 fragColour;

void main()
{
    fragColour = vec4(colour, 1);
}
