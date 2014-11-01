#version 120

varying vec2 texCoordVarying;

void main()
{
    
    texCoordVarying = gl_MultiTexCoord0.xy;
	gl_Position = ftransform();
    
    //vec4 srcCol = texture2DRect(tex0, texCoordVarying);
    //vec4 col = gl_Color;
    //gl_FrontColor = col;
    
}