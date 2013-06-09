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

   float offset = v_texCoord.x;

   float greyness = 1. - analpha;
   float colorness = 1. - greyness;

    vec4 normalColor = texture2D(u_texture, fract(vec2(offset, v_texCoord.y)));

    float gray = greyness * (0.299*normalColor.r + 0.587*normalColor.g + 0.114*normalColor.b);

   gl_FragColor = (2. + sin(timeratio * 3.)) * vec4(gray + normalColor.r * colorness, gray + normalColor.g * colorness, gray + normalColor.b * colorness, analpha * normalColor.a);
}

