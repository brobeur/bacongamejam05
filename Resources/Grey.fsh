#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform float greyness;
//uniform float u_time;

void main()
{
/*
   float speed = 1.;
   float height = 1. - v_texCoord.y;
   float offset = pow(height, 2.5);
   offset *= (sin(u_time * speed) * .2);
*/
   float colorness = 1. - greyness;

    vec4 normalColor = texture2D(u_texture, v_texCoord);
   //    vec4 normalColor = texture2D(u_texture, fract(vec2(v_texCoord.x + offset, v_texCoord.y))).rgb;

    float gray = greyness * (0.299*normalColor.r + 0.587*normalColor.g + 0.114*normalColor.b);


   gl_FragColor = vec4(gray + normalColor.r * colorness, gray + normalColor.g * colorness, gray + normalColor.b * colorness, normalColor.a);
   // gl_FragColor = vec4(gray , gray, gray, normalColor.a);
}