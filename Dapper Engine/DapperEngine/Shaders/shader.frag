#version 330

in vec4 vColor;
in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D theTexture;
uniform vec2 customMinUV;
uniform vec2 customMaxUV;
uniform vec4 color;

void main()
{
    //texture color + (uniform color * uniform_alpha)
    
	vec2 size = customMaxUV - customMinUV;
	vec2 modboi = mod(TexCoord, size) + customMinUV;
	outColor = (texture(theTexture, modboi) * color);
	if (outColor.a < 0.1)
	{
		discard;
	}
}