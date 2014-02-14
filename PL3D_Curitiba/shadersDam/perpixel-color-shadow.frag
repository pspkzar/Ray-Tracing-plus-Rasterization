#version 330

uniform sampler2DShadow shadowMap;
uniform vec4 diffuse;

in vec4 projShadowCoord;
in vec3 normalV, lightDir;

out vec4 outColor;

void main()
{
	vec4 color = diffuse * 0.25; // ambient term
	
	vec3 n = normalize (normalV);
	
	float NdotL = max(0.0,dot (n, lightDir));
	if (NdotL > 0.01) {
		//float d = texture(shadowMap, vec2(projShadowCoord)).r;
		//if (d < projShadowCoord.z)
		if (textureProj (shadowMap, projShadowCoord) < 1.0)
			color += diffuse  * NdotL * textureProj (shadowMap, projShadowCoord);
		else
			color += diffuse  * NdotL;
		
	}
	
	//outColor = color;
	outColor = vec4(1, 1, 1, 1);
}
