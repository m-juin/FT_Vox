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
layout(location = 2) in int inVertTexture;

layout(location = 0) out vec3 outVertColor;
layout(location = 1) out int outVertText;

void main()
{
    gl_Position = camera.view * camera.proj * obo.model * vec4(inVertPos, 1.0);
    outVertColor = inVertColor;
    outVertText = inVertTexture;
}

