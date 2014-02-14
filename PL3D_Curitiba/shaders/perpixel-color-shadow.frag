#version 330

uniform sampler2DShadow shadowMap;
uniform sampler2D texUnit;
uniform vec4 diffuse;
uniform int texCount;

in vec2 texCoordV;
in vec4 projShadowCoord;
in vec3 normalV, lightDir;

out vec4 outColor;

void main()
{
	vec4 color, diff;

	if (texCount > 1)
		diff = diffuse * texture(texUnit, texCoordV) ; // ambient term
	else 
		diff = diffuse;
		
	color = diff * 0.25;
	
	vec3 n = normalize (normalV);
	
	float NdotL = max(0.0,dot (n, lightDir));
	
	if (NdotL > 0.01) {

		if (textureProj (shadowMap, projShadowCoord) < 1.0) 
			color += diff  * NdotL * textureProj (shadowMap, projShadowCoord) ;
		else
			color += diff  * NdotL;
		
	}
	
	outColor = color;	
}
