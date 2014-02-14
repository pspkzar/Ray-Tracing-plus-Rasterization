attribute float triangleID;
varying float col;

void main()
{
	col = triangleID;
	gl_Position = ftransform();
} 