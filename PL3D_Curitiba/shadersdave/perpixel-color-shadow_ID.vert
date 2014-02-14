uniform vec4 lightDirection;
uniform vec4 lightColor;
uniform mat4 lightSpaceMat;


varying vec4 projShadowCoord,eyePos;
varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

attribute vec4 triangleID;
varying float triangle, mesh;
varying vec4 coords;

void main()
{
	triangle = triangleID[1];
	mesh = triangleID[0];

	//normal = normalize (gl_NormalMatrix * gl_Normal);

	normal = normalize(gl_Normal);
	//lightDir = normalize(vec3(1.0,1.0,1.0));
	lightDir = normalize(vec3(-lightDirection));
	//lightDir = normalize(vec3(gl_ModelViewMatrix * vec4(-lightDirection,0))) ;
	
	//lightDir = normalize (lightPosition);
	

	diffuse = gl_FrontMaterial.diffuse * vec4 (lightColor);
	ambient = gl_FrontMaterial.ambient;
		
	projShadowCoord = lightSpaceMat * gl_Vertex;
	//coords = gl_ModelViewMatrix * gl_Vertex; // coordenadas de câmara
	coords = gl_Vertex; // coordenadas do mundo
	gl_Position = ftransform();
} 
