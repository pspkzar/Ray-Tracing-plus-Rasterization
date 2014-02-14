#version 330

uniform mat4 PVM;
uniform mat3 NormalMatrix;
uniform float x;

in vec4 position;
in vec4 normal;
in vec4 texCoord0;

out vec4 vertexPos;
out vec2 TexCoord;
out vec3 Normal;
out float X;

void main()
{
	Normal = normalize(NormalMatrix * vec3(normal));	
	X = x;
	TexCoord = vec2(texCoord0);
//	Normal = normalize(vec3(normal));
	gl_Position = PVM * position;
}