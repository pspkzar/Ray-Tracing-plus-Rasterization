uniform sampler2D textureUnit;
uniform ivec4 texSize;
uniform int index;

varying vec2 tc0;

void main(void)
{
   float offset = 1.0/float(texSize[index]);
   vec4 c  = texture2D(textureUnit, tc0);
   vec4 bl = texture2D(textureUnit, tc0 + vec2(-offset, -offset));
   vec4 l  = texture2D(textureUnit, tc0 + vec2(-offset,     0.0));
   vec4 tl = texture2D(textureUnit, tc0 + vec2(-offset,  offset));
   vec4 t  = texture2D(textureUnit, tc0 + vec2(    0.0,  offset));
   vec4 ur = texture2D(textureUnit, tc0 + vec2( offset,  offset));
   vec4 r  = texture2D(textureUnit, tc0 + vec2( offset,     0.0));
   vec4 br = texture2D(textureUnit, tc0 + vec2( offset,  -offset));
   vec4 b  = texture2D(textureUnit, tc0 + vec2(    0.0, -offset));
   //gl_FragColor = vec4((c-((bl+tl+l+ur+br+r+t+b)*0.125)).r,0,0,1) ;
//		gl_FragColor =  c - (bl + l + tl + t + ur + r + br + b) * 0.125;
   if (length(c - (bl + l + tl + t + ur + r + br + b) * 0.125) > 0.01)
		gl_FragColor = vec4(1.0,1.0,1.0,1.0);
	else 
		gl_FragColor = vec4(0.0,0.0,0.0,1.0);
}