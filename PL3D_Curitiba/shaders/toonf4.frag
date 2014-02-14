varying vec3 ld,normal;
uniform vec3 threshold;
uniform float step1, step2, step3, step4;
uniform vec3 baseColor;

void main()
{

	float intensity;
	vec4 color;
	vec3 n = normalize(normal);
	
	intensity = dot(ld,n);
	
	if (intensity > threshold.z)
		color = vec4(baseColor,1.0) * step1;
	else if (intensity > threshold.y)
		color = vec4(baseColor,1.0) * step2;
	else if (intensity > threshold.x)
		color = vec4(baseColor,1.0) * step3;
	else
		color = vec4(baseColor,1.0) * step4;
	
	gl_FragColor = color;
} 
