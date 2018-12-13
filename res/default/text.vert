#version 330 core

layout (location = 0) in vec2 coord;
layout (location = 1) in vec2 src;

out vec2 texCoord;
uniform mat4 ortho;

void main()
{
    gl_Position = ortho * vec4(coord, 0.0, 1.0);
    texCoord = src;
}