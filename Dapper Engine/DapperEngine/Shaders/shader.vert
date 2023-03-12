#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec4 vColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 device;
uniform mat4 view;

uniform vec2 customU = vec2(1.0, 0.0);
uniform vec2 customV = vec2(1.0, 0.0);

void main()
{
	// Maybe try moving this math to the fragment shader, it may be more accurate for text coordinate calculations.
    vec2 theUV = vec2(tex.x * (customU.x-0.001) + customU.y, tex.y * (customV.x-0.001) + customV.y);
	TexCoord = theUV;
	gl_Position = device * view * model * vec4(pos, 1.0);
	vColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}