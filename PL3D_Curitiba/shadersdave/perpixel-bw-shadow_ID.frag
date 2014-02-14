uniform sampler2DShadow shadowMap;

uniform vec4 x;

varying vec4 projShadowCoord;
varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

varying float triangle, mesh;
varying vec4 coords;

void main()
{
	//vec4 color = ambient;
	vec4 color = vec4(0,0,0,0);
	vec3 n = normalize (normal);
	
	//float NdotL = max(0.0,dot (n, lightDir));
	float NdotL = dot (n, lightDir);// * 100000;
	if (NdotL > 0.1) {
		//if (shadow2DProj (shadowMap, projShadowCoord).r) {
		//	color += diffuse  * NdotL * shadow2DProj (shadowMap, projShadowCoord).r ;
		if (shadow2DProj(shadowMap, projShadowCoord).r > 0.0)
			color = vec4(1.0,1.0,1.0,0.0);
		//color = shadow2DProj(shadowMap, projShadowCoord).r;
		//}
	}
	///NOVO - para acomodar 0.1 do if
	else
	{
		NdotL = 0.0;
	}



	gl_FragData[0] = color;	
	//gl_FragData[0] = vec4(n,0.0);
	gl_FragData[1] = coords;
	//gl_FragData[1] = vec4(lightDir,0.0);
	//gl_FragData[2] = vec4(mesh,triangle,0.0,0.0);
	////NOVO - trocado o de cima pelo debaixo///
	gl_FragData[2] = vec4(mesh,triangle,NdotL,0.0);
	//gl_FragColor = color;
}
