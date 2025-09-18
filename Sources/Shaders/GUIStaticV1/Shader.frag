#version 450

layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec4 inTexColor;
layout(location = 2) in flat int inTexMode;

layout(set = 0, binding = 0) uniform sampler2D textureAtlas;
layout(set = 0, binding = 1) uniform sampler2D fontAtlas;
layout(set = 0, binding = 2) uniform sampler2D dynamicTextures[2];

layout(push_constant) uniform AtlasRegion {
    vec2 uvMin;
    vec2 uvMax;
	vec2 atlasSize;
	int textureID;
} region;

layout(location = 0) out vec4 outColor;


void main() 
{
	vec4 text;
	if (inTexMode == 0)
	{
		float alpha = texture(fontAtlas, texCoord).r;   // masque de la font
    	text = vec4(1.0, 1.0, 1.0, alpha);  
	}
	else if (inTexMode == 1)
	{
		
		vec2 uvRange = region.uvMax - region.uvMin;
		vec2 repeatedUV;
		repeatedUV = region.uvMin + fract(texCoord) * uvRange;
		text = texture(textureAtlas, repeatedUV);
	}
	else if (inTexMode == 2 && region.textureID != -1)
	{
		text = texture(dynamicTextures[region.textureID], texCoord);
	}
	else
		text = vec4(1.0);
	outColor = text * inTexColor;
}
