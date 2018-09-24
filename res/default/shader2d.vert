#version 330 core

layout (location = 0) in vec3 currentCoord;
layout (location = 1) in vec2 currentTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float depth;

uniform int depthEnabled;

void main()
{
    if(depthEnabled == 1) {
        gl_Position = projection * view * vec4(currentCoord, depth);
    } else if(depthEnabled == 0) {
        gl_Position = vec4(currentCoord, 1.0);
    }
    TexCoord = currentTexCoord;
}