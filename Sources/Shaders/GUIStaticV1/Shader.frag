#version 450

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec4 inTexColor;

layout(binding = 0, location = 0) uniform sampler2D textureAtlas;

layout(location = 0) out vec4 outColor;

void main() 
{
	outColor = texture(textureAtlas, texCoord);

	outColor *= inTexColor;
}
