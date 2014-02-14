#version 420


uniform vec4 diffuse;
uniform vec4 specular, emission;
uniform float shininess;
uniform int texCount;
uniform sampler2D texUnit;

in vec3 normalV;
in vec3 eyeV;
in vec3 lightDirV;
in vec2 texCoordV;
in float intensityV, intSpecV;

out vec4 colorOut;

void main() {

	vec4 dif;
	vec4 spec;


	spec = specular * pow(intSpecV,shininess);
	dif = diffuse ;
	
	if (texCount == 0) {
		colorOut = (intensityV + 0.3) * dif + emission + spec ;
	}
	else {
		colorOut = ((intensityV + 0.3) * dif + emission) * texture(texUnit, texCoordV) + spec;
	}
	
	//colorOut = (intensity + 0.2) * dif + spec ;
	
}