#version 420


uniform vec3 diffuse;
uniform vec3 specular;

in vec3 normalV;
in vec3 eyeV;
in vec3 lightDirV;
in vec2 texCoordV;

out vec4 colorOut;

void main() {

	vec3 dif;
	vec3 spec;

	float intensity = max(dot(normalize(normalV),lightDirV), 0.0);


	vec3 h = normalize(lightDirV + normalize(eyeV));	
	float intSpec = max(dot(h,normalize(normalV)), 0.0);
	spec = specular * pow(intSpec,100);
	dif = diffuse * intensity;
	
	colorOut = vec4(0.2, 0.8, 0.4, 1.0); //(intensity + 0.2) * dif + spec ;
	
}