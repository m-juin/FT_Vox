#version 450

layout(push_constant) uniform Camera {
    mat4 view;
    mat4 proj;
} camera;

layout(binding = 0) uniform ObjectsBufferObject {
    mat4 model;
} obo;

layout(location = 0) in vec3 inVertPos;
layout(location = 1) in vec3 inVertColor;
layout(location = 2) in vec2 inVertCoord;
layout(location = 3) in uint inIsColorAffected;


layout(location = 0) out vec3 outVertColor;
layout(location = 1) out vec2 outVertCoord;
layout(location = 2) out flat uint fragIsColorAffected;

void main()
{
    gl_Position = camera.proj * camera.view * obo.model * vec4(inVertPos, 1.0);
    outVertColor = inVertColor;
    outVertCoord = inVertCoord;
    fragIsColorAffected = inIsColorAffected;
}

