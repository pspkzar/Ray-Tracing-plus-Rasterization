#version 330

uniform mat4 pUser, vUser;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 PV;
uniform mat4 PVM;
uniform vec3 lightDir;
uniform mat3 normalUser;

in vec4 position;
in vec4 normal;
in vec4 texCoord0;


out vec3 normalV;
out vec2 texCoordV;

void main() {

	mat4 p = pUser;
	gl_Position = vUser * modelMatrix * position;
	p[2][2] = - p[2][2];
	//p[2][3] = - p[2][3];
	gl_Position = p * gl_Position;
	gl_Position /= gl_Position.w;
	gl_Position.z = -gl_Position.z;
	gl_Position = PV * gl_Position;
	//gl_Position = PVM * position;
	//normalV = normalize(normalUser * vec3(normal));
	normalV = normalize(vec3(viewMatrix * modelMatrix * vec4(vec3(normal),0.0)));
	texCoordV = vec2(texCoord0);	
}