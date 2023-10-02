#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;

out vec2 UV;
uniform vec2 backgroundScale;

void main()
{
    UV = vUV;
    vec2 scaledPosition = vPos.xy * backgroundScale;
    gl_Position = vec4(scaledPosition, 1.0);
}