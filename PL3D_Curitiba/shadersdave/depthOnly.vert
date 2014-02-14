attribute vec4 triangleID;
varying float triangle, mesh;

void main(void) {
	triangle = triangleID[1];
	mesh = triangleID[0];


	vec4 v = gl_ModelViewMatrix * gl_Vertex;
	
	//v.z += 0.05;
	gl_Position = gl_ProjectionMatrix * v;
}
