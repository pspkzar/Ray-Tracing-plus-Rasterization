#version 400

uniform sampler2DShadow shadowMap[4];
//uniform float split[4];

in vec4 viewSpacePos;

in vec4 projShadowCoord[4];
in vec3 normalV, lightDir;

out vec4 outColor;

void main()
{
	vec4 color = vec4(0.4);
	vec4 diffuse = vec4(1.0);
		
	vec3 n = normalize (normalV);

	float NdotL = max (dot (n, lightDir), 0.0);
	
	if (NdotL > 0.0) {
//		color += diffuse * NdotL;
	
		
		 float split[4];
		 split[0] = 5.0;
		 split[1] = 15.0;
		 split[2] = 50.0;
		 split[3] = 150.0;	
	
		//float distance = -viewSpacePos.z /  viewSpacePos.w;
		float distance = -viewSpacePos.z;
		for (int i = 0; i < 4; i++) {
			if (distance < split[i]) {
				if (i == 0) {
					color += diffuse * (NdotL * textureProj(shadowMap[i], projShadowCoord[i])) * vec4(1.0, 0.0, 0.0, 1.0);
					//color += diffuse * NdotL * textureGather (shadowMap[i], projShadowCoord[i].xy, projShadowCoord[i].z/projShadowCoord[i].w) * vec4(1.0, 0.0, 0.0, 1.0);
				}
				else if (i == 1){
					color += diffuse * (NdotL * textureProj(shadowMap[i], projShadowCoord[i])) * vec4(0.0, 1.0, 0.0, 1.0);
				}
				else if (i == 2)
					color += diffuse * NdotL * textureProj (shadowMap[i], projShadowCoord[i]) * vec4(0.0, 0.0, 1.0, 1.0);
				else if (i == 3)
					color += diffuse * NdotL * textureProj (shadowMap[i], projShadowCoord[i]) * vec4(1.0, 0.0, 1.0, 1.0);
				//float depth = projShadowCoord[i].z / projShadowCoord[i].w;
				//float depthShadow = shadow2DProj (shadowMap[i], projShadowCoord[i]).r;
				/*if (depthShadow > depthShadow) {
					color *= 0.0;
				
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
								
				}*/
			break;		

			}
		}
	}
	outColor = color;	
}
