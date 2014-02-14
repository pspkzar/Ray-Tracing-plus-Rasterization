#version 330

uniform sampler2D color;
uniform sampler2D shadow;

in vec2 TexCoord;

out vec4 outColor;

void main() {

	vec4 color = texture(color, TexCoord);
	
	outColor = color * 	texture(shadow, TexCoord).x;
}
