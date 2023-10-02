#version 450

out vec4 FragColor;
in vec2 UV;
in float Opacity;

uniform sampler2D _Texture;

void main()
{
    vec4 texColor = texture(_Texture, UV);
    FragColor = vec4(texColor.rgb, texColor.a * Opacity); 
}