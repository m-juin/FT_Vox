#version 450

layout(location = 0) in vec3 fragVertColor;
layout(location = 1) in vec2 fragVertCoord;
layout(location = 2) in flat uint fragIsColorAffected;

layout(set = 0, binding = 1) uniform sampler2D blocksTextureAtlas;

layout(location = 0) out vec4 outColor;

void main()
{
	vec4 color = texture(blocksTextureAtlas, fragVertCoord);
	// color *= vec4(fragVertColor, 1.0);
	if (fragIsColorAffected == 1 && color.r == color.g && color.g == color.b)
		color = color * vec4(fragVertColor, 1.0);
	outColor = color;
}