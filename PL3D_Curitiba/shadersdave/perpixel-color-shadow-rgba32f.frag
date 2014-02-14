uniform sampler2D depthTexture;

varying vec4 viewSpacePos;

varying vec4 projShadowCoord[4];
varying vec4 diffuse, ambient;
varying vec3 normal, lightDir;

float doPcf (vec4 tc, int i) {
	float s = 1.0;
	vec4 lightDepth;
	float scale = 0.00048828125; //1.0 / 2048.0
	
	float depth = tc.z;
	
	lightDepth = texture2DProj (depthTexture, tc);
	
	if (lightDepth[i] > depth) {
		s = 0.0;
		s++;
	}
	lightDepth = texture2D(depthTexture, tc.st + vec2(-0.125, -0.25) * scale );
	if (lightDepth[i] > depth)
		s++;
	lightDepth = texture2D(depthTexture, tc.st + vec2(-0.375, 0.25) * scale);
	if (lightDepth[i] > depth)
		s++;
	lightDepth = texture2D(depthTexture, tc.st + vec2(0.375, -0.25) * scale);
	if (lightDepth[i] > depth)
		s++;
	lightDepth = texture2D(depthTexture, tc.st + vec2(0.125, 0.25) * scale);
	if (lightDepth[i] > depth)
		s++;
	s = s / 5.0;
		
//	 s = clamp(s + 0.5,0.0,1.0);
	 
	 return s;
}


void main() {

	
	int index;
	//vec4 tc;
	float s = 1.0;
	float distFromCam = -viewSpacePos.z /* viewSpacePos.w*/;
	
	vec4 color = ambient;
		
	vec3 n = normalize (normal);
	vec3 ld = normalize (lightDir);

	float NdotL = max (dot (n, ld), 0.0);
	
	if (NdotL > 0.0) {
		color += diffuse * NdotL;
	
		float splitPlane[4] = {5.0, 15.0, 30.0, 60.0};
	
		for (int i = 0; i < 4; i++) {
			if (distFromCam < splitPlane[i]) {
				vec4 tc = projShadowCoord[i];
				//tc.z /= tc.w;
				s = doPcf (tc, i);
				
				color *= s;
				break;			
			}
		}
	}
	
	gl_FragColor = color;
			
			
/*
	 float f = tc.z/tc.w;

	if (lightDepth > f)
		 	gl_FragColor = vec4(1,1,1,1);
	 else
		 	gl_FragColor = vec4(f,f,f,1);	
*/
}
	
