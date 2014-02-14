#version 420

in vec4 position;
//out vec4 gl_Position;

uniform mat4 PVM;

void main(void)
{
	gl_Position = PVM * position;
}
