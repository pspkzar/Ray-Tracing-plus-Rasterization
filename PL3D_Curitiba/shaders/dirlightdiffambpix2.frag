uniform vec3 lightDirection, lightColor;

varying vec3 Normal;

void main()
{
	vec4 color;
	vec4 ambient;
	float intensity;
	vec4 lightIntensityDiffuse;
	vec3 lightDir;
	vec3 n;
	
	lightDir = normalize(-lightDirection);
	n = normalize(Normal);	
	intensity = max(dot(lightDir,n),0.0);
	
	lightIntensityDiffuse = vec4(lightColor * intensity, 1.0);
	
	color = gl_FrontMaterial.diffuse;
	ambient = gl_FrontMaterial.ambient * vec4(lightColor,1.0);

	gl_FragColor = (color * lightIntensityDiffuse) + ambient;

}
