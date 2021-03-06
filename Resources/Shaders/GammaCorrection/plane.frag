#version 330 core

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	//衰减常数
	// float constant;
	// float linear;
	// float quadratic;
};

in vec2 fTexCoord;
in vec3 fNormal;
in vec3 fPosition;
in Light fLight;


uniform bool blinn;
uniform float gamma;
uniform int attenIndex;
uniform sampler2D texture_diffuse1;

out vec4 color;

void main(){
	// 环境光照
	vec3 ambientColor = fLight.ambient * vec3(texture(texture_diffuse1, fTexCoord));
	// 漫反射
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(fLight.position - fPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuseColor = fLight.diffuse * diff * vec3(texture(texture_diffuse1, fTexCoord));
	// 镜面反射
	vec3 viewDir = normalize(-fPosition);
	float spec = 0.0;
	if(blinn){
		// Blinn-Phong
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(norm, halfwayDir), 0.0), 8.0);
	} else{
		// pure Phong
		vec3 reflectDir = reflect(-lightDir, norm);
		spec = pow(max(dot(viewDir,reflectDir), 0.0), 32.0);
	}
	vec3 specularColor = fLight.specular * spec * vec3(texture(texture_diffuse1, fTexCoord));
	// 计算衰减
	float distance = length(fLight.position - fPosition);
	float attenuation = 1.0 / pow(distance, attenIndex);
	// 片段颜色
	color = vec4(attenuation * (ambientColor + diffuseColor + specularColor), 1.0f);
	// Gamma矫正
	color.rgb = pow(color.rgb, vec3(1.0/gamma));
}

