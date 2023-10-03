#version 450

out vec4 FragColor;
in vec2 UV;
in float Opacity;

uniform sampler2D characterTexture1;
uniform float iTime;
uniform vec2 iResolution;

void main()
{
    // Pixel coordinates (from 0 to 1)
    vec2 fragCoord = gl_FragCoord.xy / iResolution.xy;
    vec2 UV = (2.0 * fragCoord - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);

    float charX = 0.0;
    float charY = 0.5 * sin(iTime * 0.9) - 0.6;
   
    float distanceToFrag = length(UV - vec2(charX, charY));
    vec4 color1 = texture(characterTexture1, UV);

    // Output to screen
    FragColor = vec4(color1.rgb, color1.a * Opacity);
}