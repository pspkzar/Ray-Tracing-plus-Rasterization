#version 330

uniform sampler2D textureUnit;

in vec2 texCoordV;

out vec4 outColor;

void main(void)
{
    float offset = 1.0/1024.0;
    vec4 c  = texture2D(textureUnit, texCoordV);
    vec4 bl = texture2D(textureUnit, texCoordV + vec2(-offset, -offset));
    vec4 l  = texture2D(textureUnit, texCoordV + vec2(-offset,     0.0));
    vec4 tl = texture2D(textureUnit, texCoordV + vec2(-offset,  offset));
    vec4 t  = texture2D(textureUnit, texCoordV + vec2(    0.0,  offset));
    vec4 ur = texture2D(textureUnit, texCoordV + vec2( offset,  offset));
    vec4 r  = texture2D(textureUnit, texCoordV + vec2( offset,     0.0));
    vec4 br = texture2D(textureUnit, texCoordV + vec2( offset,  -offset));
    vec4 b  = texture2D(textureUnit, texCoordV + vec2(    0.0, -offset));

    if (length(c - (bl + l + tl + t + ur + r + br + b) * 0.125) > 0.0001)
		outColor = vec4(1.0,1.0,1.0,1.0);
	else 
		outColor = vec4(0.0,0.0,0.0,1.0);
}