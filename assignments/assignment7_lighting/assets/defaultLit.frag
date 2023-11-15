#version 450
out vec4 FragColor;

in Surface {
	vec2 UV;  //Per-fragment interpolated UV
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal
    vec3 CameraPosition;
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
uniform vec3 _CameraPosition;

void main() {
	vec4 texColor = texture(_Texture, fs_in.UV);

	//ambient lighting
	vec3 ambient = _Material.ambientK * _Color; //vec3(1);

	//diffuse + specular lighting
	vec3 lighting = vec3(0); //<--

	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		vec3 lightDirection = normalize(_Lights[i].position - fs_in.WorldPosition);

		//vec3 viewDirection = normalize(-fs_in.WorldPosition); //camera starts (0,0,0)
		vec3 viewDirection =  normalize(fs_in.CameraPosition - fs_in.WorldPosition); //camera starts (0,0,0)

		vec3 halfVector = normalize(lightDirection + viewDirection);
        vec3 reflectDirection = reflect(-lightDirection, normalize(fs_in.WorldNormal));

		float diff = max(dot(fs_in.WorldNormal, lightDirection), 0.0);
		vec3 diffuse = _Material.diffuseK * _Lights[i].color * diff;

		float spec = pow(max(dot(fs_in.WorldNormal, halfVector), 0.0), _Material.shininess); 
        vec3 specular = _Material.specular * _Lights[i].color * spec;

		//considering light intensity
		lighting += (_Lights[i].color * (diffuse + specular));
	}

    vec3 finalColor = texColor.rgb * (ambient + lighting);
	FragColor = vec4(finalColor, texColor.a);
}