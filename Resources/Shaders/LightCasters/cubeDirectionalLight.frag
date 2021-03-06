#version 330 core

//材质属性
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
//无穷远光照
struct Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in Light fLight;
in vec2 texCoords;
in vec3 vertexNormal;
in vec3 fragPos;

out vec4 color;

uniform Material material;

void main(){
	//环境光照
	vec3 ambient=fLight.ambient*vec3(texture(material.diffuse,texCoords));
	//正规法线
	vec3 norm = normalize(vertexNormal);
	//计算漫反射
	vec3 lightDir = normalize(-fLight.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = fLight.diffuse * diff * vec3(texture(material.diffuse,texCoords));
	//计算镜面反射
	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir= reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0),material.shininess);
	vec3 specular = fLight.specular * spec * vec3(texture(material.specular,texCoords));
	//返回片元颜色
	color = vec4((ambient+diffuse+specular),1.0f);
}