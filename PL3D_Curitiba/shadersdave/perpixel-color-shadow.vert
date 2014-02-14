uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform mat4 lightSpaceMat;


varying vec4 projShadowCoord,eyePos;
varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;


void main()
{
	normal = normalize (gl_NormalMatrix * gl_Normal);

	lightDir = vec3(gl_ModelViewMatrix * vec4(-lightDirection,0)) ;
	
	//lightDir = normalize (lightPosition);
	

	diffuse = gl_FrontMaterial.diffuse; // * vec4 (lightColor, 1.0);
	ambient = gl_FrontMaterial.ambient;
		
	projShadowCoord = lightSpaceMat * gl_Vertex;

	gl_Position = ftransform();
} 
