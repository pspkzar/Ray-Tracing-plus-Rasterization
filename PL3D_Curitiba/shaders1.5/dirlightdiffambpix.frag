#version 330

uniform vec4 lightDirection, lightColor;
uniform vec4 diffuse, ambient, emission;
uniform float shininess;
uniform int texCount;
uniform sampler2D texUnit;

in vec3 Normal;
in vec2 TexCoord;
in float X;
out vec4 outColor;

void main()
{
	vec4 color;
	vec4 amb;
	float intensity;
	vec4 lightIntensityDiffuse;
	vec3 lightDir;
	vec3 n;
	
	lightDir = normalize(vec3(lightDirection));
	n = normalize(Normal);	
	intensity = max(dot(lightDir,n),0.0);
	
	lightIntensityDiffuse = lightColor * intensity;
	
	if (texCount == 0) {
		color = diffuse;
		amb = ambient;
	}
	else {
		color = diffuse * texture2D(texUnit, TexCoord);
		amb = color * 0.3;
	}
	outColor = vec4(vec3((color * lightIntensityDiffuse) + amb), color.a) + emission;
//	output = (color * X) + amb;

}
