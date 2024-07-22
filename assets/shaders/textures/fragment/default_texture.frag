#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D tex;
uniform sampler2D depthTexture;
uniform sampler2D normalTexture;
uniform float outlineWidth;
uniform vec4 outlineColor;
uniform vec2 texelSize;
uniform float edgeThreshold = 0.075;


void main()
{
    vec3 normalCenter = texture(normalTexture, TexCoord).rgb;
    vec4 color = texture(tex, TexCoord);
    float depthCenter = texture(depthTexture, TexCoord).r;
    float edge = 0.0;
    vec2 newTextel = texelSize*0.2;




    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 offset = vec2(float(x), float(y)) * newTextel;
            vec3 normalSample = texture(normalTexture, TexCoord + offset).rgb;
            float depthSample = texture(depthTexture, TexCoord + offset).r;
            if (distance(normalCenter, normalSample) > edgeThreshold || abs(depthCenter - depthSample) > edgeThreshold) {
                edge = 0.3;
            }}}

    FragColor = vec4(color.rgb-vec3(edge),color.a);
}