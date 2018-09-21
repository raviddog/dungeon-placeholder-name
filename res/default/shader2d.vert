#version 330 core

layout (location = 0) in vec3 currentCoord;
layout (location = 1) in vec2 currentTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float depth;

void main()
{
    gl_Position = projection * view * vec4(currentCoord, depth);
    TexCoord = currentTexCoord;
}