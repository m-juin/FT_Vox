#version 450

layout(location = 0) in vec3 inVertPos;

layout(push_constant) uniform Camera {
    mat4 view;
    mat4 proj;
} camera;

layout(location = 0) out vec3 fragDir;

void main() {
    // Garde seulement la rotation de la matrice vue
    mat4 viewNoTrans = mat4(mat3(camera.view));
    
    // Transforme la position
    vec4 pos = camera.proj * viewNoTrans * vec4(inVertPos, 1.0);
    
    // Force z = w pour que la depth soit toujours 1.0 (le plus loin)
    gl_Position = vec4(pos.xy, pos.w, pos.w);
    
    // Passe la direction directement (pas d'inversion Y ici)
    fragDir = inVertPos;
    // fragDir.y *= -1;
}