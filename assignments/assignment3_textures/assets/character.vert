#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 fUV;

out vec2 UV;
out float Opacity;

uniform vec2 characterScale; //scale
uniform float characterOpacity; //opacity 0-1

void main()
{
    UV = fUV;
    Opacity = characterOpacity;
    vec2 scaledPosition = vPos.xy * characterScale;
    gl_Position = vec4(scaledPosition, 1.0, 1.0);
}