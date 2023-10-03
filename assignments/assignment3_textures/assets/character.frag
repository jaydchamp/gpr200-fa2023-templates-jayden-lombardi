#version 450

out vec4 FragColor;
in vec2 UV;
in float Opacity;

uniform sampler2D characterTexture1;
uniform float iTime;
uniform vec2 iResolution;

void main()
{
    vec2 wrappedUV = fract(UV);
    
    // Pixel coordinates (from 0 to 1)
    vec2 fragCoord = gl_FragCoord.xy / iResolution.xy;

    float charX = 0.0;
    float charY = 0.5 * sin(iTime * 0.9) - 0.6;
    vec4 color1 = texture(characterTexture1, wrappedUV);

    // Output to screen
    FragColor = vec4(color1.rgb, color1.a * Opacity);
}