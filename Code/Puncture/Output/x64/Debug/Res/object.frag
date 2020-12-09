#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec3 viewPos;  //观察者的位置 仅用于计算光照强度

out vec4 FragColor;  //输出的片段颜色

uniform vec4 objectColor;  //物体的颜色
uniform vec3 lightColor;  //光源的颜色

void main()
{
    //环境光
    float ambientStrength=0.2;  //环境光强度
    vec3 ambient=ambientStrength*lightColor;  //光源引起的环境光强度

    //镜面反射(类似镜面反射)
    //光强度由[法线]与[片元指向观察者的方向向量]的点乘有关
    float specularStrength=0.5;
    vec3 viewDir=normalize(viewPos-FragPos);  //由片元指向观察者的单位向量
    float spec=abs(dot(viewDir,Normal));  //法线方向与视角方向的夹角，决定了镜面反射强度系数
    vec3 specular=specularStrength*spec*lightColor;  //光源、物体表面方向、观察者位置 共同决定镜面反射强度

    //总的面片颜色
    vec3 result=(ambient+specular)*vec3(objectColor);  //组合两种光 照射到物体上
    FragColor=vec4(result,objectColor.w);


} 