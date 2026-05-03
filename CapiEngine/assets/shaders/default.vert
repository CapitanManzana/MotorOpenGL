#version 330 core
layout(location = 0) in vec3 vPos;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vTCoords;


out vec3 FragPos;
out vec3 Normal;
out vec2 vTexCoords;

uniform mat4 modelView;  // model-view matrix
uniform mat4 projection; // projection
uniform mat4 model; 
uniform mat3 normalMatrix; 

void main() {
    gl_Position = projection * modelView * vec4(vPos.x, vPos.y, vPos.z, 1.0);
    FragPos = vec3(model * vec4(vPos, 1.0));

    // Esta es la matriz normal (Normal Matriz) Es la inversa de la transpuesta de la 3x3 sup izq 
    Normal = normalMatrix * vNormal;
    vTexCoords = vTCoords;
}