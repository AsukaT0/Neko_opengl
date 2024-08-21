#version 330 core

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 CamPos;
uniform vec3 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;


out vec4 FragColorOut;
float k = 0.3;

in vec2 texCoord;
uniform sampler2D textureSampler;
uniform bool useTexture;


vec3 calcDiff(vec3 FragColor) {
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseFactor = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = (lightColor * diffuseFactor + FragColor);
	return diffuse/2;
}

vec3 calcAmbi(vec3 FragColor){
	vec3 ambient = (FragColor);
	return ambient;
}
vec4 calc(vec3 col){
	return vec4(calcAmbi(col)*(1 - k) + calcDiff(col)*k,1);
}

void main(){
	FragColorOut = calc(useTexture?
						vec3(texture(textureSampler, texCoord).rgb):
						FragColor);
}
