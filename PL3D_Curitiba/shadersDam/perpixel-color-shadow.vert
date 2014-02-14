#version 330

uniform vec4 lightDirection;
uniform mat4 lightSpaceMat;

uniform mat4 PVM;
uniform mat4 V, M;
uniform mat3 NormalMatrix;

in vec4 position;
in vec4 normal;

out vec4 projShadowCoord;
//out vec4 pos;
out vec3 normalV;
out vec3 lightDir;


void main() 
{
	normalV = normalize (NormalMatrix * vec3(normal));

	lightDir = normalize (vec3(V * -lightDirection)) ;
			
	projShadowCoord = lightSpaceMat * M * position;

	gl_Position = PVM * position;
} 