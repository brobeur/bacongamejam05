#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float timeratio; // 0 to 1
uniform float analpha;

void main()
{

   float speed = 10.;

   float targetH = abs(sin(timeratio * .2 + sin(timeratio * .9)));
   float height = abs(targetH - v_texCoord.y);
   float offset = pow(height, 2.5);
   offset *= (sin(timeratio * speed + sin(timeratio * .2)) * .1);
   // clamp the offset to prevent wrapping
   offset = clamp(offset + v_texCoord.x, 0., 1.);
   float squish = abs(sin(timeratio * 3. + sin(timeratio * 2.)));

   float greyness = 1. - analpha;
   float colorness = 1. - greyness + .5;

    vec4 normalColor = texture2D(u_texture, fract(vec2(offset, v_texCoord.y * squish + v_texCoord.y * (1.0 - squish) * .5)));

    float gray = greyness * (0.299*normalColor.r + 0.587*normalColor.g + 0.114*normalColor.b);

   gl_FragColor = vec4(gray + normalColor.r * colorness, gray + normalColor.g * colorness, gray + normalColor.b * colorness, analpha * normalColor.a);
}

