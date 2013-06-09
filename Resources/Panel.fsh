#ifdef GL_ES
precision mediump float;
#endif

#define M_PI 3.1415926535897932384626433832795

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float timeratio; // 0 to 1

void main()
{
   // swirl it up
   float inverse = 1. - timeratio;
   float offsetX = inverse * .2 * sin(2. * M_PI * (timeratio + v_texCoord.y));
   float offsetY = 2. * (v_texCoord.y - .5) * sin(2. * M_PI * timeratio);

    vec4 normalColor = texture2D(u_texture, fract(vec2(v_texCoord.x - offsetX, v_texCoord.y)));

   gl_FragColor = normalColor;
}

