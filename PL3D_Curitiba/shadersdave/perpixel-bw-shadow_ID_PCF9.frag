uniform sampler2DShadow shadowMap;

uniform vec4 x;

varying vec4 projShadowCoord;
varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

varying float triangle, mesh;
varying vec4 coords;

void main()
{
	const float offset = 1.0 / 2048.0;
	//const float offset = 1.0 / 1024.0;
    //const float offset = 1.0 / 512.0;
    //const float offset = 1.0 / 256.0;
    
    vec3 shadowColect1 = vec3(0,0,0);
    vec3 shadowColect2 = vec3(0,0,0);
    vec3 shadowColect3 = vec3(0,0,0);
    float result;
   
	vec4 color = vec4(0,0,0,0);
	vec3 n = normalize (normal);
	
	float NdotL = dot (n, lightDir);// * 100000;
	if (NdotL > 0.1) {
		if (shadow2DProj(shadowMap, projShadowCoord + vec4(-offset, -offset, 0.0, 0.0)).r > 0.0)
			shadowColect1.x = 1.0;
		if (shadow2DProj(shadowMap, projShadowCoord + vec4(    0.0, -offset, 0.0, 0.0)).r > 0.0)
			shadowColect1.y = 1.0;
		if (shadow2DProj(shadowMap, projShadowCoord + vec4( offset, -offset, 0.0, 0.0)).r > 0.0)
			shadowColect1.z = 1.0;
		if (shadow2DProj(shadowMap, projShadowCoord + vec4(-offset,     0.0, 0.0, 0.0)).r > 0.0)
			shadowColect2.x = 1.0;
		if (shadow2DProj(shadowMap, projShadowCoord).r > 0.0)
			shadowColect2.y = 1.0;
		if (shadow2DProj(shadowMap, projShadowCoord + vec4( offset,     0.0, 0.0, 0.0)).r > 0.0)
			shadowColect2.z = 1.0;
		if (shadow2DProj(shadowMap, projShadowCoord + vec4(-offset,  offset, 0.0, 0.0)).r > 0.0)
			shadowColect3.x = 1.0;
		if (shadow2DProj(shadowMap, projShadowCoord + vec4(    0.0,  offset, 0.0, 0.0)).r > 0.0)
			shadowColect3.y = 1.0;
		if (shadow2DProj(shadowMap, projShadowCoord + vec4( offset,  offset, 0.0, 0.0)).r > 0.0)
			shadowColect3.z = 1.0;
		
		result = (shadowColect1.x + shadowColect1.y + shadowColect1.z
			+ shadowColect2.x + shadowColect2.y + shadowColect2.z
			+ shadowColect3.x + shadowColect3.y + shadowColect3.z)/9.0;
				
		color = vec4(result,result,result,0.0);
	}
	///NOVO - para acomodar 0.1 do if
	else
	{
		NdotL = 0.0;
	}



	gl_FragData[0] = color;	
	gl_FragData[1] = coords;
	//gl_FragData[2] = vec4(mesh,triangle,0.0,0.0);
	////NOVO - trocado o de cima pelo debaixo///
	gl_FragData[2] = vec4(mesh,triangle,NdotL,0.0);
}
