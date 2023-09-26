#version 450
in vec2 UV;
out vec4 FragColor;

uniform float sunY;
uniform vec3 sunColor = vec3(1.0, 0.3, 0.0);
uniform vec3 skyColor = vec3(0.0, 0.0, 0.5);
    uniform float sunRadius = 0.25;
uniform float hillLine = -0.7;
uniform float iTime;
uniform vec2 iResolution;

void main() {
    // Pixel coordinates (from 0 to 1)
    vec2 fragCoord = gl_FragCoord.xy / iResolution.xy;
    vec2 UV = (2.0 * fragCoord - 1.0) * vec2(iResolution.x / iResolution.y, 1.0);

    // Sun horz. pos (CONSTANT)
    float sunX = 0.0;

    // Sun vert. pos - (UP + DOWN)
    float sunY = 0.5 * sin(iTime * 0.9) - 0.6;

    // Fragment to the Sun
    float distToSun = length(UV - vec2(sunX, sunY));

    // Sun Gradient (GRADIENT)(Dark Blue[NightTime]->LightBlue/Purple[DayTime])
    vec3 sunColor = mix(vec3(1.0, 0.3, 0.0), vec3(1.0, 0.8, 0.2), smoothstep(-0.05, 0.05, distToSun));

    // Calculate the sky gradient (Orange[Night] -> Pink[Day])
    vec3 skyColor = mix(vec3(0.0, 0.0, 0.5), vec3(1.0, 0.8, 2.0), smoothstep(0.0, 1.0, sunY + 0.5));

    // Blurry Sun Edge (SmoothStep) 
    float sunRadius = 0.25;
    float edge = smoothstep(sunRadius, sunRadius + 0.02, distToSun);
    sunColor *= edge;

    // Rolling Hills (Location + Movement)
    float hillLine = 0.1 * sin((UV.x - iTime) * 5.0) - 0.7;

    // Combine
    vec3 finalColor = mix(skyColor, sunColor, sunColor);
    finalColor = mix(finalColor, vec3(0.0, 0.1, 0.2), step(UV.y, hillLine));

    // Output to screen
    FragColor = vec4(finalColor, 1.0);
}