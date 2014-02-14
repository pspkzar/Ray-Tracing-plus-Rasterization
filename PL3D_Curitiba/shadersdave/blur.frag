uniform sampler2D textureUnit;

void main(void)
{
   //mudar conforme resolução do viewport
   //const float offset = 1.0 / 1024.0;
   //const float offset = 1.0 / 512.0;
   const float offset = 1.0 / 256.0;
   vec2 texCoord = gl_TexCoord[0].xy;
   vec4 c  = texture2D(textureUnit, texCoord);
   vec4 bl = texture2D(textureUnit, texCoord + vec2(-offset, -offset));
   vec4 l  = texture2D(textureUnit, texCoord + vec2(-offset,     0.0));
   vec4 tl = texture2D(textureUnit, texCoord + vec2(-offset,  offset));
   vec4 t  = texture2D(textureUnit, texCoord + vec2(    0.0,  offset));
   vec4 ur = texture2D(textureUnit, texCoord + vec2( offset,  offset));
   vec4 r  = texture2D(textureUnit, texCoord + vec2( offset,     0.0));
   vec4 br = texture2D(textureUnit, texCoord + vec2( offset, -offset));
   vec4 b  = texture2D(textureUnit, texCoord + vec2(    0.0, -offset));
   //gl_FragColor = vec4((c-((bl+tl+l+ur+br+r+t+b)*0.125)).r,0,0,1) ;
   gl_FragColor =  vec4(((c + bl + l + tl + t + ur + r + br + b) / 9.0).r,0.0,0.0, 1.0);
   //gl_FragColor = vec4(1.0,1.0,0.0,1.0);
}

/*void main(void)
{
   //const float offset = 1.0 / 1024.0;
   const float offset = 1.0 / 256.0;
   float num = 0, total = 0;
   vec2 texCoord = gl_TexCoord[0].xy;
   vec4 c  = texture2D(textureUnit, texCoord);
   if(texCoord.x != 0.0 && texCoord.y != 0.0)
   {
      vec4 bl = texture2D(textureUnit, texCoord + vec2(-offset, -offset));
      total+=bl;
      num++;
   }
   if(texCoord.x != 0.0)
   {
      vec4 l  = texture2D(textureUnit, texCoord + vec2(-offset,     0.0));
      total+=l;
      num++;
   }
   if(texCoord.x != 0.0 && texCoord.y != 255.0)
   {
      vec4 tl = texture2D(textureUnit, texCoord + vec2(-offset,  offset));
      total+=tl;
      num++;
   }
   if(texCoord.y != 255.0)
   {
      vec4 t  = texture2D(textureUnit, texCoord + vec2(    0.0,  offset));
      total+=t;
      num++;
   }
   if(texCoord.x != 255.0 && texCoord.y != 255.0)
   {
      vec4 ur = texture2D(textureUnit, texCoord + vec2( offset,  offset));
      total+=ur;
      num++;
   }
   if(texCoord.x != 255.0)
   {
      vec4 r  = texture2D(textureUnit, texCoord + vec2( offset,     0.0));
      total+=r;
      num++;
   }
   if(texCoord.x != 255.0 && texCoord.y != 0.0)
   {
      vec4 br = texture2D(textureUnit, texCoord + vec2( offset, -offset));
      total+=br;
      num++;
   }
   if(texCoord.y != 0.0)
   {
      vec4 b  = texture2D(textureUnit, texCoord + vec2(    0.0, -offset));
      total+=b;
      num++;
   }
   //gl_FragColor = vec4((c-((bl+tl+l+ur+br+r+t+b)*0.125)).r,0,0,1) ;
   gl_FragColor =  vec4((total / num).r,0.0,0.0, 1.0);
   //gl_FragColor = vec4(1.0,1.0,0.0,1.0);
}*/