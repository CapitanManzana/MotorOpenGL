#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal; // necesitas normales en tu mesh

uniform mat4 modelView;
uniform mat4 projection;

void main() {
    float outlineThickness = 0.05;
    vec3 expanded = aPos + aNormal * outlineThickness;
    gl_Position = projection * modelView * vec4(expanded, 1.0);
}