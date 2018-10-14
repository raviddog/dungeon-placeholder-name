#version 330 core

layout (location = 0) in vec2 coord;
layout (location = 1) in float alpha;

out float oalpha;

void main()
{
    gl_Position = vec4(coord, 0.0, 1.0);
    oalpha = alpha;
}