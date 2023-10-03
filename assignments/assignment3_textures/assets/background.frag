#version 450

out vec4 FragColor;
in vec2 UV;

uniform sampler2D backgroundTexture1;
uniform sampler2D backgroundTexture2;
uniform vec2 backgroundScale;
uniform float iTime; 
uniform float scrollSpeedX; 
uniform float scrollSpeedY; 

void main()
{
    float scrollOffsetX = sin(iTime * scrollSpeedX);
    float scrollOffsetY = cos(iTime * scrollSpeedY);

    vec2 fUV = (UV + vec2(scrollOffsetX, scrollOffsetY)) * backgroundScale;

    fUV = fract(fUV);

    vec4 color1 = texture(backgroundTexture1, fUV);
    vec4 color2 = texture(backgroundTexture2, fUV); 
    FragColor = mix(color1, color2, 0.5); // interpolation
}