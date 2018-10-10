#version 330 core

in VertexData
{
    vec3 position;
    vec3 normal;
    vec3 eyeDirection;
    vec3 lightDirection;
    vec3 lightPosition;
} inData;

uniform vec3 materialColour;

out vec4 fragColour;

vec3 shadedColour()
{
    vec3 lightColour = vec3(1, 1, 1);
    float lightPower = 50.0;

    vec3 materialDiffuseColour = vec3(0.71, 0.44, 0.19);
    vec3 materialAmbientColour = vec3(0.5, 0.5, 0.5) * materialDiffuseColour;
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
        cosTheta / (dist * dist) + 
        materialSpecularColour * lightColour * lightPower * pow(cosAlpha, 3) /
        (dist * dist);
}

void main()
{
    fragColour = vec4(shadedColour(), 1.0);
}
