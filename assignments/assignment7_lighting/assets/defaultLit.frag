#version 450
out vec4 FragColor;

in Surface {
	vec2 UV;  //Per-fragment interpolated UV
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal
}fs_in;

struct Light
{
	vec3 position;
	vec3 color;
};

struct Material 
{
    float ambientK;
    float diffuseK;
    float specular;
    float shininess;
};

#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];
uniform sampler2D _Texture;
uniform vec3 _Color;
uniform Material _Material;

void main() {
	vec4 texColor = texture(_Texture, fs_in.UV);

	//ambient lighting
	vec3 ambient = _Material.ambientK * vec3(1); //<-- adjust ambient level here

	//diffuse + specular lighting
	vec3 lighting = vec3(1); //<--

	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		//diffuse reflection calc
		vec3 lightDirection = normalize(_Lights[i].position - fs_in.WorldPosition);
        float diff = max(dot(fs_in.WorldNormal, lightDirection), 0.0);
		vec3 diffuse = _Material.diffuseK * _Lights[i].color * diff;

		//specular reflection calc (phong)
		vec3 viewDirection = normalize(-fs_in.WorldPosition); //camera starts (0,0,0)
        vec3 reflectDirection = reflect(-lightDirection, fs_in.WorldNormal);
        float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0);  //<-- adjust specular level here
        vec3 specular = _Material.specular * _Lights[i].color * spec;

        lighting += diffuse + specular;
	}

    vec3 finalColor = texColor.rgb * (ambient + lighting);
	FragColor = vec4(finalColor, texColor.a);
}