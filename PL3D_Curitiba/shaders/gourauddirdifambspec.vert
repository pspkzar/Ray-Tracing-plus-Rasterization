#version 420

uniform mat3 NormalMatrix;
uniform mat4 PVM, VM;

uniform vec4 lightDirection;

in vec4 position;
in vec4 normal;
in vec4 texCoord0;



out vec2 texCoordV;
out float intensityV, intSpecV;

void main () {
	
	texCoordV = vec2(texCoord0);

	vec3 n = normalize(NormalMatrix * vec3(normal));
	vec3 l = normalize(-vec3(lightDirection));
	
	intensityV = max(dot(n,l), 0.0);

	vec3 pos = vec3(VM * position);
	vec3 eye = normalize(-pos);
	
	vec3 h = normalize(l + normalize(eye));	
	intSpecV = max(dot(h,normalize(n)), 0.0);


	gl_Position = PVM * position;	
}