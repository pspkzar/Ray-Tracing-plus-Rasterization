varying vec3 Normal;

void main()
{
	vec4 color;
	vec4 ambient;
	float intensity;
	vec4 lightIntensityDiffuse;
	vec3 lightDir;
	vec3 n;
	
	lightDir = normalize(vec3(gl_LightSource[0].position));
	n = normalize(Normal);	
	intensity = max(dot(lightDir,n),0.0);
	
	lightIntensityDiffuse = gl_LightSource[0].diffuse * intensity;
	
	color = gl_FrontMaterial.diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;

	gl_FragColor = (color * lightIntensityDiffuse) + ambient;

}
