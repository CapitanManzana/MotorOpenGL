#version 330 core
layout (location = 0) in vec3 aPos;

// Lo que tu motor ya le pasa
uniform mat4 modelView; 
uniform mat4 projection;

// EL NUEVO UNIFORM: Pásale solo el model para saber dónde está en el mundo
uniform mat4 model; 

out vec3 WorldPos;

void main() {
        // Y usamos tu matriz combinada para la posición en pantalla
    gl_Position = projection * modelView * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    // Calculamos la posición en el mundo para el Fragment Shader
    WorldPos = vec3(model * vec4(aPos, 1.0));

}