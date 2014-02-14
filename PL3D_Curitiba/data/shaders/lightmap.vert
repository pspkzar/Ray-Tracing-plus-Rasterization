varying vec2 LightMapCoords;

void
main()
{
  gl_Position = ftransform();
  gl_FrontColor = gl_FrontMaterial.diffuse;
  LightMapCoords = vec2(gl_MultiTexCoord0);
}
