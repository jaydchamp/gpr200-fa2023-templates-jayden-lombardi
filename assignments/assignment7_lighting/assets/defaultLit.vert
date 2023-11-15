#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out Surface{
	vec2 UV;
	vec3 WorldPosition;
	vec3 WorldNormal;
	vec3 CameraPosition;
}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
	vs_out.UV = vUV;

	//vs_out.WorldPosition
	vs_out.WorldPosition = (vec4(vPos, 1.0) * _Model).xyz;

	//vs_out.WorldNormal (see Normal Matrix in slides)
	mat3 normalMatrix = transpose(inverse(mat3(_Model)));
    vs_out.WorldNormal = normalize(normalMatrix * vNormal);

    vs_out.CameraPosition = (_Model * vec4(0.0, 0.0, 0.0, 1.0)).xyz;

	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}