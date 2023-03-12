#version 330 core
layout (location = 0) in vec3 vertex; // <vec2 position, vec2 texCoords>
layout (location = 1) in vec2 texCoords;


uniform mat4 device;
uniform mat4 model;
uniform mat4 view;

void main()
{
    gl_Position = device * view * model * vec4((vertex.xy), vertex.z, 1.0);
}