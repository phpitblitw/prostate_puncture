#version 330 core
layout (location = 0) in vec3 aPos;  //顶点位置
layout (location = 1) in vec3 aNormal;  //当前顶点法向量

out vec3 FragPos;  //片元位置
out vec3 Normal;
out vec3 viewPos;

uniform mat4 model;  //模型矩阵
uniform mat4 view;  //观察矩阵
uniform mat4 projection;  //投影矩阵
uniform vec3 viewerPos;  //观察者的位置 仅用于计算光照强度

void main()
{
    FragPos=vec3(model*vec4(aPos,1.0));
    viewPos=vec3(inverse(view)*vec4(viewerPos,1.0));  //随着观察矩阵的变化 改变观察者的位置
    Normal=mat3(transpose(inverse(model)))*aNormal;  //法线矩阵被定义为「模型矩阵左上角的逆矩阵的转置矩阵」 为了滤除model对法线的影响

    gl_Position = projection*view*model*vec4(aPos, 1.0);    
}