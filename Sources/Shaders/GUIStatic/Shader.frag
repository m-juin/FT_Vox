#version 450

layout(binding = 1) uniform sampler2D textures[2048];

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec4 inTexColor;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform imgType
{
	int isGrayScale;
	uint texIndex;
} type;

void main() 
{
	vec4 texColor;
	if (type.isGrayScale != 2)
		texColor = texture(textures[type.texIndex], texCoord);
	else
		texColor = vec4(1.0, 1.0, 1.0, 1.0);
	if (type.isGrayScale == 1)
		texColor.rgb = vec3(texColor.r, texColor.r, texColor.r);
	texColor = texColor * inTexColor;
    outColor = texColor;
}
