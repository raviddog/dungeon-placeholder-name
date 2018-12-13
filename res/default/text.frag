#version 330 core

//in vec2 textCoord;

uniform vec2 textCoord;

out vec4 FragColor;
uniform vec4 textColor;
uniform sampler2D font_img;

void main()
{
    FragColor = textColor * texture(font_img, textCoord);
}