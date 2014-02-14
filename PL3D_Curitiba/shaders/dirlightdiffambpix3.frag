uniform vec4 lightDirection, lightColor;
uniform vec4 diffuse, ambient;
uniform float shininess;
uniform bool enabled;
uniform int enumprop;
uniform sampler2D texUnit;
uniform int texUnit2;


varying vec3 Normal;
varying vec2 tc0;

void main()
{
//int texUnit2=1024;
	vec4 color;
	float intensity;
	vec4 lightIntensity;
	vec3 lightDir;
	vec3 n;
	
	if (enabled) {
		lightDir = normalize(vec3(-lightDirection));
		n = normalize(Normal);	
		intensity = max(dot(lightDir,n),0.0);
	//	intensity = max(dot(vec3(0.0,0.0,1.0),n),0.0);
		
		lightIntensity = lightColor * intensity;
		
		gl_FragColor = diffuse * lightIntensity + ambient * 0.25;
		if (texUnit2 > 512)
			gl_FragColor = gl_FragColor * texture2D(texUnit,tc0.st);
		else
			gl_FragColor = texture2D(texUnit,tc0.st);
		
	//	gl_FragColor =  vec4(n,1.0) ;
	}
	else
	if (enumprop == 0)
		gl_FragColor = vec4(0.0,1.0,0.0,1.0);
	else if (enumprop == 1)
		gl_FragColor = vec4(1.0,0.0,0.0,1.0);
	else if (enumprop == 2)
		gl_FragColor = vec4(0.0,0.0,1.0,1.0);
}
