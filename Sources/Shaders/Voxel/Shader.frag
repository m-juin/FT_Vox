#version 450

layout(location = 0) in vec3 fragVertColor;
layout(location = 1) in flat int fragVertText;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(fragVertColor, 1.0);
}