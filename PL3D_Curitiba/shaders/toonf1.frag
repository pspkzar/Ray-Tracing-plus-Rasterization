#version 150

in vec3 LightDir,Normal;
out vec4 output;

void main()
{
	float intensity;
	vec4 color;
	
	vec3 n = normalize(Normal);	
	intensity = dot(LightDir,n);
	
	if (intensity > 0.95)
		color = vec4(0.5,0.5,1.0,1.0);
	else if (intensity > 0.5)
		color = vec4(0.3,0.3,0.6,1.0);
	else if (intensity > 0.25)
		color = vec4(0.2,0.2,0.4,1.0);
	else
		color = vec4(0.1,0.1,0.2,1.0);
	
	output = color;
} 
