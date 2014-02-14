#version 330

uniform vec4 diffuse;
uniform vec4 ambient;
uniform vec4 lightDir;
uniform vec4 emission;

in vec3 normalV;
in vec2 texCoordV;

out vec4 outColor;

void main() {

	vec3 l = normalize(vec3(lightDir));
	//l = normalize(vec3(1.0,1.0,1.0));
	vec3 n = normalize(vec3(normalV));
	float i = max(dot(l,n),0.0);

	outColor = vec4(i)*diffuse + ambient + emission;

}