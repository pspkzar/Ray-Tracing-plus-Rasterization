uniform vec3 lightPosition;
uniform vec3 lightColor;


varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;


void main()
{
	normal = normalize (gl_NormalMatrix * gl_Normal);
	
	lightDir = normalize (vec3(gl_ModelViewMatrix * vec4(lightPosition, 0.0)));
	//lightDir = normalize (lightPosition);

	diffuse = gl_FrontMaterial.diffuse * vec4 (lightColor, 1.0);
	ambient = 0.4 * gl_FrontMaterial.ambient * vec4 (lightColor, 1.0);

	gl_Position = ftransform();
} 