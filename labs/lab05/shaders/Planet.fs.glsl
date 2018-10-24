#version 330 core

in VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec3 eyeDirection;
    vec3 lightDirection;
    vec3 lightPosition;
} inData;

uniform sampler2D planetTexture;

out vec4 fragColour;

vec3 shadedColour()
{
    vec3 lightColour = vec3(1, 1, 1);
    float lightPower = 1.0;

    vec2 uv = inData.texCoord;
    uv.y = -uv.y;
    vec3 materialDiffuseColour = texture(planetTexture, uv).rgb;
    return lightPower * lightColour * materialDiffuseColour;

}

void main()
{
    fragColour = vec4(shadedColour(), 1.0);
}
