uniform sampler2DShadow shadowMap;

uniform vec4 x;

varying vec4 projShadowCoord;
varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

void main()
{
	vec4 color = ambient;
	vec3 n = normalize (normal);
	
	float NdotL = max(0.0,dot (n, lightDir));
	if (NdotL > 0.0) {
		//if (shadow2DProj (shadowMap, projShadowCoord).r) {
			color += diffuse  * NdotL * shadow2DProj (shadowMap, projShadowCoord).r ;
		//}
	}
	

			
	gl_FragColor = color;	
}
