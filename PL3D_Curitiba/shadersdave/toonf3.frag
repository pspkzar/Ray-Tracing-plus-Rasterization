varying vec3 ld,normal;
uniform vec3 threshold;

void main()
{

	float intensity;
	vec4 color;
	vec3 n = normalize(normal);
	
	intensity = dot(ld,n);
	
	if (intensity > threshold.z)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > threshold.y)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > threshold.x)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);
	
	gl_FragColor = color;
} 
