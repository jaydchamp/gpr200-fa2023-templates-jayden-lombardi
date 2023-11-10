#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;  //Per-fragment interpolated UV
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal
}fs_in;

struct Light
{
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];

uniform sampler2D _Texture;

void main(){
	FragColor = texture(_Texture,fs_in.UV);
	//TODO: Lighting calculations using corrected normal
}