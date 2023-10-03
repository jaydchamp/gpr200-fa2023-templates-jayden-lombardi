#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;

out vec2 UV;
out float Opacity;

uniform vec2 characterScale; //scale
uniform float characterOpacity; //opacity 0-1
uniform float iTime;

void main()
{
    UV = vUV;
    Opacity = characterOpacity;

    float characterPosX = iTime * 0.2;
    float characterPosY = 0.5 * sin(iTime * 0.9) - 0.6;

    //vec2 scaledPosition = vec2(vPos.x + characterPosX, vPos.y + characterPosY) * characterScale;
      vec2 scaledPosition = vec2(vPos.x * characterScale.x + characterPosX, vPos.y * characterScale.y + characterPosY);
    //scaledPosition += vec2(characterPosX, characterPosY);

    gl_Position = vec4(scaledPosition, vPos.z, 1.0);
}