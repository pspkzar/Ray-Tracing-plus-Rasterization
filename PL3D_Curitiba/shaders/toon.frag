varying vec3 normal, lightDir, ecPos;

void main()
{
	vec4 diffuse,ambient,specular,color;
	float intensity,shininess,aux;
	vec3 n,hv,ec;

	n = normalize(normal);
	intensity = max(dot(lightDir,n),0.0); 
	diffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
	ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	color = ambient + diffuse*intensity;
	color += gl_FrontMaterial.ambient * gl_LightModel.ambient;
	color += gl_FrontMaterial.emission;
	if (intensity > 0.0) {
		ec = normalize(-ecPos); 
		hv = normalize(ec+lightDir);
		shininess = pow(max(dot(hv,n),0.0),gl_FrontMaterial.shininess); 
		specular = shininess * gl_LightSource[0].specular * gl_FrontMaterial.specular;
//		color += specular;
	}
	vec4 c0 = vec4(0.0);
	vec4 c1 = vec4(0.0,0.2,0.6,1.0);
	vec4 c2 = vec4(0.0,0.4,0.8,1.0);
	aux = length(vec3(color));
	if (shininess > 0.0)
		color = vec4(1.0,1.0,1.0,1.0)*0.8;
	else if (aux < 0.5)
		color = c0;	
	else if (aux < 0.8)
		color = c1;
	else
		color = c2;		
		

	gl_FragColor = color;
}
