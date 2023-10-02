#version 450

out vec4 FragColor;
in vec2 UV;

uniform sampler2D backgroundTexture1;
uniform sampler2D backgroundTexture2;

uniform vec2 scrollOff;
uniform vec2 backScale;

void main()
{
    vec2 fUV = UV * backgroundScale + scrollOffset;

    fUV = fract(fUV);

    vec4 color1 = texture(backgroundTexture1, fUV);
    vec4 color2 = texture(backgroundTexture2, flUV);
    FragColor = mix(color1, color2, 0.5); //interpolation
}