#ifdef GL_ES
precision highp float;
#endif
uniform float iGlobalTime;
uniform vec2 iResolution;
void main( void )
{
  vec2 uPos = (gl_FragCoord.xy / iResolution.xy);
  float speed = 42.0;
  float intensity = 0.0;
  float ratio = iResolution.x / iResolution.y;
  float count_x = (5.0);
  float count_y = (count_x / ratio);
  float a = (2.0 * 3.14);
  intensity = step(0.9, (sin(mix(0.0, a * count_x, uPos.x))));
  intensity += step(0.9, (sin(mix(0.0, a * count_y, uPos.y) + (iGlobalTime * speed))));

  vec3 color = vec3(0.0, 1.0, 1.0);
  gl_FragColor = vec4(color * min(255.0, intensity), 1.0);
}
