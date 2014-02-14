
uniform mat4 PVM;

attribute vec4 position;
attribute vec4 texCoord0;

varying vec2 tc0;



void main()
{
	tc0 = vec2(texCoord0);
	gl_Position = PVM * position;
} 