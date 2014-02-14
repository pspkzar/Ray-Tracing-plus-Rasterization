uniform sampler2DShadow shadowMap;

uniform vec4 x;

varying vec4 projShadowCoord;
varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

void main()
{
	//vec4 color = ambient;
	vec4 color = vec4(0,0,0,0);
	vec3 n = normalize (normal);
	
	float NdotL = max(0.0,dot (n, lightDir));
	if (NdotL > 0.0) {
		//if (shadow2DProj (shadowMap, projShadowCoord).r) {
		//	color += diffuse  * NdotL * shadow2DProj (shadowMap, projShadowCoord).r ;
		if (shadow2DProj(shadowMap, projShadowCoord).r > 0.0)
			color = vec4(1.0,1.0,1.0,0.0);
		//color = shadow2DProj(shadowMap, projShadowCoord).r;
		//}
	}
	

			
	gl_FragColor = color;	
}
