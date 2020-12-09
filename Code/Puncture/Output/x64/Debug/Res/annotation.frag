#version 330 core
out vec4 FragColor;  //输出的片段颜色

uniform vec4 color;  //面片颜色

void main()
{
    FragColor = color;
} 