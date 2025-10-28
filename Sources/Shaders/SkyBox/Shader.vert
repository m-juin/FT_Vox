#version 450

layout(location = 0) in vec3 inVertPos;

layout(push_constant) uniform Camera {
    mat4 view;
    mat4 proj;
} camera;

layout(location = 0) out vec3 fragDir;

void main() {
    mat4 viewNoTrans = mat4(mat3(camera.view)); // garde juste la rotation
    vec4 pos = camera.proj * viewNoTrans * vec4(inVertPos, 1.0);
    gl_Position = pos.xyww;
    fragDir = mat3(viewNoTrans) * inVertPos;
    fragDir.y = -fragDir.y;
}