#version 150

uniform mat4 viewMatrix, projMatrix;

in vec4 position;
in vec3 normal;

out vec3 LightDir,Normal;

void main()
{
	LightDir = normalize(vec3(1.0,1.0,1.0));
	Normal = normalize(normal);

	gl_Position = projMatrix * viewMatrix * position ;
} 
