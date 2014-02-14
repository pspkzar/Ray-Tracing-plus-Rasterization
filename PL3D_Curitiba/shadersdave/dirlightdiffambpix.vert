varying vec3 Normal;
varying vec2 tc0;

uniform mat4 PVM;
uniform mat3 NormalMatrix;

attribute vec4 position;
attribute vec3 normal;
attribute vec3 texCoord0;

void main()
{
	tc0 = vec2(texCoord0);
	Normal = normalize(NormalMatrix * normal);	
	gl_Position = PVM * vec4(vec3(position * 2.0),1.0);
//	gl_Position = PVM * vec4(vec3(gl_Vertex * 20.0),1.0);
//	gl_Position = ftransform();
} 
