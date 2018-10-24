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

const vec3 diffuseMaterial = vec3(0.64, 0.64, 0.64);
const vec3 specularMaterial = vec3(0.99, 0.99, 0.99);
const vec3 ambientMaterial = vec3(1.0, 1.0, 1.0);
const vec3 lightPos = vec3(50.0, 50.0, 50.0);

const vec4 metalGray = vec4(0.0, 0.46, 0.69, 1.0);

vec3 shadedColour()
{
    vec3 lightColour = vec3(1, 1, 1);
    float lightPower = 800.0;

    vec2 uv = inData.texCoord;
    uv.y = -uv.y;
    vec3 materialDiffuseColour = texture(planetTexture, uv).rgb;
    vec3 materialAmbientColour = vec3(0.1, 0.1, 0.1) * materialDiffuseColour;
    vec3 materialSpecularColour = vec3(0.3, 0.3, 0.3);

    // Distance to light.
    float dist = length(inData.lightPosition - inData.position);

    // Normal of the fragment.
    vec3 n = normalize(inData.normal);

    // Direction of the light.
    vec3 l = normalize(inData.lightDirection);
    float cosTheta = clamp(dot(n, l), 0, 1);

    // Eye vector.
    vec3 E = normalize(inData.eyeDirection);
    vec3 R = reflect(-l, n);
    float cosAlpha = clamp(dot(E, R), 0, 1);

    return materialAmbientColour + 
        materialDiffuseColour * lightColour * lightPower * 
        cosTheta / (dist * dist);
}

void main()
{
    fragColour = vec4(shadedColour(), 1.0);
}
