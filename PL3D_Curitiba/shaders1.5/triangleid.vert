#version 150

uniform mat4 PVM;

in int triangleID;
flat out int col;

void main()
{
	col = triangleID;
	gl_Position = PVM * position;
} 