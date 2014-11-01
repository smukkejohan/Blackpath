#version 120

uniform sampler2DRect tex0;
uniform vec2 aTexSize;
uniform vec2 bTexSize;
uniform float fadeToB;
uniform sampler2DRect tex1;

varying vec2 texCoordVarying;


void main()
{

    
    vec4 a = texture2DRect(tex0, texCoordVarying * aTexSize);
    vec4 b = texture2DRect(tex1, texCoordVarying * bTexSize); //normalized?
    
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    //color = 1 * mix(one,two, 1.0);
    
    color = mix(a, b, fadeToB);

    gl_FragColor = color;
}