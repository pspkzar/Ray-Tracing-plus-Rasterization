

uniform sampler2DShadow shadowMap[4];
uniform float split[4];

varying vec4 viewSpacePos;

varying vec4 projShadowCoord[4];
varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

void main()
{
	vec4 color = ambient;
		
	vec3 n = normalize (normal);
	vec3 ld = normalize (lightDir);

	float NdotL = max (dot (n, ld), 0.0);
	
	if (NdotL > 0.0) {
//		color += diffuse * NdotL;
	
		
		float splitPlane[4];
splitPlane[0] = 5.0;
splitPlane[1] = 15.0;
splitPlane[2] = 50.0;
splitPlane[3] = 500.0;	
		
		float distance = -viewSpacePos.z /  viewSpacePos.w;
		//int i = 0;
		for (int i = 0; i < 4; i++) {
			if (distance < split[i]) {
		
				color += diffuse * NdotL* shadow2DProj (shadowMap[i], projShadowCoord[i]);
				/*float depth = projShadowCoord[i].z / projShadowCoord[i].w;
				float depthShadow = shadow2DProj (shadowMap[i], projShadowCoord[i]).r;
				if (depth > depthShadow) {
					color *= 0.0;*/
				/*
					if (0 == i) {
						color = vec4 (1.0, 0.0, 0.0, 1.0);
					}				
					else if (1 == i) {
						color = vec4 (0.0, 1.0, 0.0, 1.0);
					}				
					else if (2 == i) {
						color = vec4 (0.0, 0.0, 1.0, 1.0);
					}				
					else if (3 == i) {
						color = vec4 (0.0, 1.0, 1.0, 1.0);
					}
				*/				
				/*}*/
				break;
			}
		}
	}
	gl_FragColor = color;	
}