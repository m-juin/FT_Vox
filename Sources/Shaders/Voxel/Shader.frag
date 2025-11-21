#version 450

layout(location = 0) in vec3 fragVertColor;
layout(location = 1) in vec2 fragVertCoord;
layout(location = 2) in flat uint fragIsColorAffected;

layout(set = 0, binding = 1) uniform sampler2D blocksTextureAtlas;
layout(set = 0, binding = 2) uniform sampler2D blocksMaskAtlas;

layout(location = 0) out vec4 outColor;

void main()
{
	if (fragIsColorAffected == 1)
	{
		outColor = vec4(fragVertColor, 1.0);
		return;
	}
	vec4 color = texture(blocksTextureAtlas, fragVertCoord);
	vec4 maskColor = texture(blocksMaskAtlas, fragVertCoord);
	if (maskColor.r != 0)
		color = color * vec4(fragVertColor, 1.0);
	outColor = color;
}