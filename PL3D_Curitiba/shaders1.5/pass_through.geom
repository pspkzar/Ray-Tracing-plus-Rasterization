/*! \file geometryPassthrough.glsl
 *  \author Jared Hoberock
 *  \brief Test shader.
 */
 
 #version 150
 
layout(triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec3 Normal[3];
in vec4 vertexPos[3];
out vec3 Normal2;


 void main()
{
  for(int i = 0; i < 3; i++)
  {
     // copy position
    gl_Position = gl_in[i].gl_Position;
	Normal2 = Normal[i];
 
    // done with the vertex
    EmitVertex();
  }
}
// a passthrough geometry shader for color and position
//void main()
//{
//  for(int i = 0; i < gl_VerticesIn; ++i)
//  {
    // copy color
//    gl_FrontColor = gl_FrontColorIn[i];
 
    // copy position
//    gl_Position = gl_PositionIn[i];
 
    // done with the vertex
//    EmitVertex();
//  }
//}