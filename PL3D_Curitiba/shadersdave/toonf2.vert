uniform vec3 lightDir;

varying vec3 ld,normal;

void main()
{
	ld = - normalize(gl_NormalMatrix * lightDir);
	ld = normalize(lightDir);
	normal = normalize(gl_NormalMatrix * gl_Normal);

	gl_Position = ftransform();
} 
