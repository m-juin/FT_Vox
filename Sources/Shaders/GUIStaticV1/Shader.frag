#version 450

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec4 inTexColor;

layout(set = 0, binding = 0) uniform sampler2D textureAtlas;
layout(set = 0, binding = 1) uniform sampler2D fontAtlas;

layout(location = 0) out vec4 outColor;

void main() 
{
	vec4 text = texture(textureAtlas, texCoord);
	outColor = text * inTexColor;
}
