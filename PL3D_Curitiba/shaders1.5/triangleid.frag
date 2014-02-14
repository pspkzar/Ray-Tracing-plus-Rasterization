#version 150

flat in float col;
out vec4 output;

void main(void)
{
   
   output =  vec4(col * 0.07,0.0,0.0, 1.0);
   //gl_FragColor = vec4(1.0,1.0,0.0,1.0);
}