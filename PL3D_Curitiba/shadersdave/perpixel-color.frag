varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

void main()
{
	vec4 color = ambient;
	
	vec3 n = normalize (normal);
	
	float NdotL = max (dot (n, lightDir), 0.0);
	
	if (NdotL > 0.0) {
		color += diffuse * NdotL;
	}
	
	gl_FragColor = color;	
}