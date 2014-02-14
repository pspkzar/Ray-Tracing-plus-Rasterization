uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform mat4 lightSpaceMat[4];

varying vec4 viewSpacePos;
varying vec4 projShadowCoord[4];
varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

void main() {

	normal = normalize (gl_NormalMatrix * gl_Normal);
	
	lightDir = normalize (vec3(gl_ModelViewMatrix * vec4(lightPosition, 0.0)));

	diffuse = gl_FrontMaterial.diffuse * vec4 (lightColor, 1.0);
	ambient = 0.3 * gl_FrontMaterial.ambient * vec4 (lightColor, 1.0);
	
	for (int i = 0; i < 4; i++) {
		projShadowCoord[i] = lightSpaceMat[i] * gl_Vertex;
	}

	viewSpacePos = gl_ModelViewMatrix * gl_Vertex;
		
	gl_Position = ftransform();
}
