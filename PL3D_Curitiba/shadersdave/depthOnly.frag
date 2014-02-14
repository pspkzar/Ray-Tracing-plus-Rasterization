varying float triangle, mesh;

void main(void) {
	gl_FragColor =  vec4(mesh,triangle,0.0,0.0);  //vec4(gl_FragCoord.z);
}
