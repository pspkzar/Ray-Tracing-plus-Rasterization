#version 420

layout (std140) uniform Matrices {
	mat4 projModelViewMatrix;
	mat4 projectionMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 modelViewMatrix;
	mat3 normalMatrix;
};


uniform vec3 lightDir;

in vec4 position;
in vec3 normal;
in vec2 texCoord;


out vec3 normalV;
out vec2 texCoordV;
out vec3 eyeV;
out vec3 lightDirV;

void main () {
	
	texCoordV = texCoord;

	normalV = normalize(normalMatrix * normal);
	lightDirV = normalize(lightDir);

	vec3 pos = vec3(modelViewMatrix * position);
	eyeV = normalize(-pos);

	gl_Position = projModelViewMatrix * position;	
}