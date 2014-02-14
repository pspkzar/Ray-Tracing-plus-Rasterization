varying vec3 Normal;

uniform mat4 PVM;
uniform mat3 NormalMatrix;

void main()
{
	Normal = normalize(NormalMatrix * gl_Normal);	
	gl_Position = PVM * gl_Vertex;
} 
