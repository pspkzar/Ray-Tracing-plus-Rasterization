uniform sampler2D LightMapSampler;
uniform vec3 LightControl;
varying vec2 LightMapCoords;

void
main()
{
  vec4 AmbientColor = 0.2 * vec4(1.0, 1.0, 1.0, 1.0);
  vec4 LightColor = vec4 (LightControl.rgb * (texture2D (LightMapSampler, LightMapCoords)).rgb, 1.0);
  gl_FragColor = /*AmbientColor*/ + gl_Color * LightColor;
}
